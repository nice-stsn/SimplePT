#include "XmlLoader.h"
#include "tinyxml2.h"
#include "Light.h"


bool loadXmlFile(
    const std::string& filePath,
    std::vector<LightInfo>& lights_info,
    Position3& eye,
    Position3& lookat,
    Vector3& up,
    double& fovy, 
    unsigned int& width, 
    unsigned int& height)
{
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError eResult = doc.LoadFile(filePath.c_str());
    if (eResult != tinyxml2::XML_SUCCESS)
    {
        return false;
    }

    // Camera
    tinyxml2::XMLElement* cameraElement = doc.FirstChildElement("camera");
    if (cameraElement)
    {
        cameraElement->QueryUnsignedAttribute("width", &width);
        cameraElement->QueryUnsignedAttribute("height", &height);
        cameraElement->QueryDoubleAttribute("fovy", &fovy);

        tinyxml2::XMLElement* eyeElement = cameraElement->FirstChildElement("eye");
        if (eyeElement)
        {
            eye.m_x = atof(eyeElement->Attribute("x"));
            eye.m_y = atof(eyeElement->Attribute("y"));
            eye.m_z = atof(eyeElement->Attribute("z"));
        }

        tinyxml2::XMLElement* lookatElement = cameraElement->FirstChildElement("lookat");
        if (lookatElement)
        {
            lookat.m_x = atof(lookatElement->Attribute("x"));
            lookat.m_y = atof(lookatElement->Attribute("y"));
            lookat.m_z = atof(lookatElement->Attribute("z"));
        }

        tinyxml2::XMLElement* upElement = cameraElement->FirstChildElement("up");
        if (upElement)
        {
            up.m_x = atof(upElement->Attribute("x"));
            up.m_y = atof(upElement->Attribute("y"));
            up.m_z = atof(upElement->Attribute("z"));
        }
    }

	tinyxml2::XMLElement* lightElement = doc.FirstChildElement("light");
	while (lightElement)
	{
		LightInfo light_info;

		light_info.m_light_mtl_name = lightElement->Attribute("mtlname");

		std::string strRadiance = lightElement->Attribute("radiance");
		size_t pos = strRadiance.find(",");
		light_info.m_radiance.m_x = atof(strRadiance.substr(0, pos).c_str());
		size_t pos2 = strRadiance.find(",", pos + 1);
		light_info.m_radiance.m_y = atof(strRadiance.substr(pos + 1, pos2 - pos - 1).c_str());
		light_info.m_radiance.m_z = atof(strRadiance.substr(pos2 + 1).c_str());

		lights_info.push_back(light_info);

		lightElement = lightElement->NextSiblingElement("light");
	}

    return true;
}
