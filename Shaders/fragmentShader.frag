#version 330 core
// in vec4 vertexColor;
// in vec2 TexCoord;
struct Material{
    vec3 ambient;
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_diffuse4;
    sampler2D texture_diffuse5;
    sampler2D texture_diffuse6;
    sampler2D texture_diffuse7;
    sampler2D texture_diffuse8;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
    sampler2D texture_specular3;
    sampler2D texture_specular4;
    sampler2D texture_specular5;
    sampler2D texture_specular6;
    sampler2D texture_specular7;
    sampler2D texture_specular8;
    float shininess;
};


struct LightDirectional{ //定义光源信息
    vec3 pos;
    vec3 dirToLight;
    vec3 color;
};

struct LightPoint{
    vec3 pos;
    vec3 color;
    vec3 dirToLight;
    float constant;
    float linear;
    float quadratic;
};

struct LightSpot{
    vec3 pos;
    vec3 color;
    vec3 dirToLight;
    float constant;
    float linear;
    float quadratic;
    float cosInnerPhy;
    float cosOuterPhy;
    
};
uniform LightPoint lightP0;
uniform LightPoint lightP1;
uniform LightPoint lightP2;
uniform LightPoint lightP3;


uniform Material material;
uniform LightDirectional lightD;
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
//uniform sampler2D ourTexture;
//uniform sampler2D ourFace;
uniform vec3 objColor;
uniform vec3 lightColor;
uniform LightPoint lightP;
uniform LightSpot lightS;
//uniform vec3 lightPos;
uniform vec3 lightDirUniform;
uniform vec3 lightPos;
uniform vec3 ambientColor;
uniform vec3 cameraPos;
out vec4 FragColor;
vec3 CalcLightDirectional(LightDirectional light, vec3 uNormal, vec3 dirToCamera){
    vec3 result;
    //diffuse
    float diffInt = max(dot(light.dirToLight, uNormal), 0);
    vec3 diffColor = diffInt * light.color * texture(material.texture_diffuse1, TexCoord).rgb;
    //specular
    vec3 R = normalize(reflect(-light.dirToLight, uNormal));
    float specInt = pow(max(dot(R, dirToCamera), 0), material.shininess);
    vec3 specColor = specInt * light.color * texture(material.texture_specular1, TexCoord).rgb;
    
    result = diffColor + specColor;
    return result;
}

vec3 CalcLightPoint(LightPoint light, vec3 uNormal, vec3 dirToCamera){
    vec3 result;
    // attenuation
    float dist = length(light.pos-FragPos);
    float attenuation = 1/(light.constant + light.linear*dist + light.quadratic * (dist*dist));
    
    // diffuse
    float diffInt = max(dot(normalize(light.pos-FragPos), uNormal), 0) * attenuation;
    vec3 diffColor = diffInt * light.color * texture(material.texture_diffuse1, TexCoord).rgb;
    
    
    //specular
    vec3 R = normalize(reflect(-normalize(light.pos-FragPos), uNormal));
    float specInt = pow(max(dot(R, dirToCamera), 0), material.shininess);
    vec3 specColor = specInt * light.color * texture(material.texture_specular1, TexCoord).rgb;
    
    result = diffColor + specColor;
    return result;
}

vec3 CalcLightSpot(LightSpot light, vec3 uNormal, vec3 dirToCamera){
    vec3 result = vec3(0, 0, 0);
    // attenuation
    float dist = length(light.pos-FragPos);
    float attenuation = 1/(light.constant + light.linear*dist + light.quadratic * (dist*dist));
    
    float spotRatio;
    float cosTheta = dot(normalize(FragPos-light.pos), -light.dirToLight);
    if (cosTheta > lightS.cosInnerPhy){
        spotRatio = 1.0f;
    }
    else if (cosTheta > light.cosOuterPhy){
        spotRatio = (cosTheta-light.cosOuterPhy) / (light.cosInnerPhy-light.cosOuterPhy);
    }
    else{
        spotRatio = 0;
    }
    // diffuse
    float diffInt = max(dot(normalize(light.pos-FragPos), uNormal), 0) * attenuation * spotRatio;
    vec3 diffColor = diffInt * light.color * texture(material.texture_diffuse1, TexCoord).rgb;
    
    
    //specular
    vec3 R = normalize(reflect(-normalize(light.pos-FragPos), uNormal));
    float specInt = pow(max(dot(R, dirToCamera), 0), material.shininess) * spotRatio;
    vec3 specColor = specInt * light.color * texture(material.texture_specular1, TexCoord).rgb;
    
    
    result += (diffColor + specColor);
    return result;
}

void main()
{
/*---------------------前次结果-------------------------------------
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 diffuse = texture(material.diffuse, TexCoord).rgb * max(dot(lightDir, Normal), 0.0) * lightColor; //漫反射
    // vec3 diffuse = texture(material.diffuse, TexCoord).rgb;
    vec3 reflectVec = reflect(-lightDir, Normal); //反射光线角度
    vec3 cameraVec = normalize(cameraPos - FragPos);
    vec3 specularAmount = texture(material.specular, TexCoord).rgb * pow(max(dot(reflectVec, cameraVec), 0), material.shininess) * lightColor;
   //FragColor = mix(texture(ourTexture, TexCoord), texture(ourFace, TexCoord), 0.2);
    //FragColor = vec4(objColor * lightColor, 1.0f) * mix(texture(ourTexture, TexCoord), texture(ourFace, TexCoord), 0.2);
    // FragColor = vec4(objColor * lightColor, 1.0f);
    //vec3 diffuse = (1.0, 1.0, 1.0);
    vec3 ambient = texture(material.diffuse, TexCoord).rgb * ambientColor;
    float cosTheta = dot(normalize(FragPos - lightPos), -1.0f * lightDirUniform);
    float spotRatio;
    if (cosTheta>lightS.cosInnerPhy){
        spotRatio = 1.0f;
        FragColor = vec4((ambient + (diffuse + specularAmount) * spotRatio) * objColor, 1.0);
    }
    else if(cosTheta > lightS.cosOuterPhy){
        spotRatio = (cosTheta-lightS.cosOuterPhy)/(lightS.cosInnerPhy-lightS.cosOuterPhy);
        FragColor = vec4((ambient + (diffuse + specularAmount) * spotRatio) * objColor, 1.0);
    }
    else{
        spotRatio = 0.0f;
        FragColor = vec4((ambient + (diffuse + specularAmount) * spotRatio) * objColor, 1.0);
    }
*/ 
    vec3 finalResult = vec3(0, 0, 0);
    vec3 uNormal = normalize(Normal);
    vec3 dirToCamera = normalize(cameraPos - FragPos);
    
    finalResult += CalcLightDirectional(lightD, uNormal, dirToCamera);
    finalResult += CalcLightPoint(lightP0, uNormal, dirToCamera);
    finalResult += CalcLightPoint(lightP1, uNormal, dirToCamera);
    finalResult += CalcLightPoint(lightP2, uNormal, dirToCamera);
    finalResult += CalcLightPoint(lightP3, uNormal, dirToCamera);
    finalResult += CalcLightSpot(lightS, uNormal, dirToCamera);
    FragColor = vec4(finalResult, 1.0f);
    //FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    
}
