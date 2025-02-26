# SimplePT



2023图形学冬学期作业，实现了一个简单的C++基于蒙特卡洛路径追踪方法。

# 第三方库

- 图片的读写：[stb/stb_image.h at master · nothings/stb (github.com)](https://github.com/nothings/stb/blob/master/stb_image.h) 
- obj 和 mtl 的加载：[tinyobjloader/tinyobjloader: Tiny but powerful single file wavefront obj loader (github.com)](https://github.com/tinyobjloader/tinyobjloader)
- xml 文件的解析：[leethomason/tinyxml2: TinyXML2 is a simple, small, efficient, C++ XML parser that can be easily integrated into other programs. (github.com)](https://github.com/leethomason/tinyxml2)

# 蒙特卡洛积分结构

- 将半球上的积分分成三个部分：指向光源的、（如果有高光项则）朝向高光项的、其余的
  - 分别实现了对于面光源的Sample和积分，对于高光项的和对于漫反射项的BRDF积分
  - 好处是对于Cornell box这样的简单光源结构，比直接采样半球积分收敛的更快；缺点是对于VeachMIS这个例子没有办法同时满足对于光源的重要性采样和对于BRDF（主要是specular）的重要性采样

# 加速结构

- 实现了简单划分的BVH，对光线求交有比较明显的加速
- 对于面数最多的 bathroom 场景构建bvh的时间在 5秒以内，是可以接受的时间。

# 材质和表现

## Cornell box

- 实现了漫反射的BRDF积分，使用cos权重的蒙特卡洛采样，结果比使用均匀半球的采样更好
- 结果比较理想，很快能得到收敛结果
- 图片：spp100

![final_check_spp100](.\SimplePT\images\final_check_spp100.png)

## Veach MIS

- 实现了基于Phong模型的高光项的BRDF积分
- 但是由于没有实现多重重要性采样，结果和reference图片仍有高光差距
- 对光源按照radiance积分
  - 实现了对于Light sample的时候的按照radiance进行重要性采样。
  - 如果仅仅对于光源如果用 Area 进行蒙特卡洛采样，在MIS图中左边光线会更暗。

![final_check_mis_spp100](.\SimplePT\images\final_check_mis_spp100.png)

## Bathroom

- 实现了 mirror 的情况处理，融合进了brdf的积分框架
- 实现了对于材质的导入和处理
- 由于光源被遮挡，当前积分框架很难有很好的收敛效果

![final_check_bathroom_spp100](.\SimplePT\images\final_check_bathroom_spp100.png)

# 问题与将来改进

1. 对于MIS和bathroom场景仍可以寻找更好的计算方法（如双向路径追踪），解决当前积分框架下的难以收敛问题
2. 程序运行效率方面，尽管当前的实现在运行'bathroom'场景时所需时间较多，可能需要几分钟，但该渲染引擎有着巨大的优化空间。
   1. 例如，可以使用空间存储结构来提高运行时的查询速度。
   2. 另外虽然实现了基本的BVH，但仍可以通过进一步优化BVH来提高运行速度。
3. 其他的补充，光线遮挡判定的 epsilon 为手动设置，对不同场景灵活性不好。纹理贴图有小错误。 可以增加折射材质。

# 感受总结

对于蒙特卡洛积分和路径追踪，虽然在上课的时候认为理论能够听懂，但是在实际写代码实现的时候，才能把很多不理解的地方真正实现并理解，上课的时候讲到的各种渲染问题虽然理论简单，但是实际实现仍有很多代码工作量。这个项目的结构有很多设计不够好的地方，希望后续有时间可以再深入优化。

# 参考资料

- [Ray Tracing in One Weekend Series](https://raytracing.github.io/)
- [BRDF光照模型与渲染方程 – PULUO](https://puluo.top/brdf光照模型与渲染方程/)



