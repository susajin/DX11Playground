
Texture2D g_Texture : register( t0 );
Texture2D g_ShadowMap : register(t1);

SamplerState g_SamplerState : register( s0 );
SamplerState g_ShadowMapSamler : register(s1);

struct LIGHT
{
    float4 Direction;
    float4 Diffuse;
    float4 Ambient;
};

struct MATERIAL
{
    float4 Diffuse;
    float4 Specular;
    float4 Emission;
};

cbuffer LightBuffer : register(b0)
{
    LIGHT Light;
}

cbuffer MaterialBuffer : register(b1)
{
    MATERIAL Material;
}

cbuffer CameraBuffer : register(b2)
{
    float4 CameraPosition;
}

struct PixelOut
{
	float4 color : SV_Target0;
};

//=============================================================================
// ピクセルシェーダ
//=============================================================================
PixelOut main(	in float2 inTexCoord	    : TEXCOORD0,
				in float4 inDiffuse	        : COLOR0,
				in float4 inPosition	    : SV_POSITION,
				in float4 inNormal		    : NORMAL0,
                in float3 inWorldPosition   : TEXCOORD1,
                in float4 inLightPosition   : TEXCOORD2,
			    in float  depth		        : DEPTH)
{
    PixelOut pixel = (PixelOut) 0;
    float bias;
    float4 color;
    float2 projectTexCoord;
    float depthValue;
    float lightDepthValue;
    float lightIntensity;

    // Set The Bias Value For Fixing The Floating Point Precision Issues
    bias = 0.001f;

    // Set The Default Output Color To Be Black (Shadow)
    color = float4(0.0f, 0.0f, 0.0f, 1.0f);

    // Calculate The Projected Texture Coordinates
    projectTexCoord.x = inLightPosition.x / inLightPosition.w / 2.0f + 0.5f;
    projectTexCoord.y = -inLightPosition.y / inLightPosition.w / 2.0f + 0.5f;

    // Determine If The Projected Coordinates Are In The [0,1] Range. If So Then This Pixel Is In The View Of The Light
    if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
    {
        // Sample The Shadow Map Depth Value From The Depth Texture Using The Sampler At The Projected Texture Coordinate Location
        depthValue = g_ShadowMap.Sample(g_ShadowMapSamler, projectTexCoord).r;

        // Calculate The Depth Of The Light
        lightDepthValue = inLightPosition.z / inLightPosition.w;

        // Subtract The Bias From The LightDepthValue
        lightDepthValue = lightDepthValue - bias;

        // Compare The Depth Of The Shadow Map Value And The Depth Of The Light To Determine Whether To Shadow Or To Light This Pixel
        // If The Light Is In Front Of The Object Then Light The Pixel, If Not Then Shadow This Pixel Since An Object (Occluder) Is Casting A Shadow On It
        if (lightDepthValue < depthValue)
        {
            // Determine The Final Diffuse Color Based On The Diffuse Color And The Amount Of Light Intensity
            color = float4(1.0f, 1.0f, 1.0f, 1.0f);
        }
    }
    
    pixel.color = color;
    return pixel;
    
	//PixelOut pixel = (PixelOut)0;
    //
    //// texture + vertex color + material color
    //pixel.color = g_Texture.Sample( g_SamplerState, inTexCoord );
	//pixel.color *= inDiffuse * Material.Diffuse;
    //
    //// lighting
    //inNormal = normalize(inNormal);
    //float light = 0.5 - 0.5 * dot(Light.Direction.xyz, inNormal.xyz);
    //
    //pixel.color.rgb *= light * Light.Diffuse;
    //pixel.color.rgb += Light.Ambient;
    //
    //// specular blinn-phong
    //float3 eyev = inWorldPosition.xyz - CameraPosition.xyz;
    //eyev = normalize(eyev);
	//
    //float3 halfv = eyev + Light.Direction.xyz;
    //halfv = normalize(halfv);
	//
    //float specular = -dot(halfv, inNormal.xyz);
    //specular = saturate(specular);
    //specular = pow(specular, 60);
	//
    //pixel.color.rgb += specular * Material.Specular;
    //
    //// fog
	////pixel.color.rgb = lerp(float3(1,1,1), pixel.color.rgb, 1 - depth);
    //
	//return pixel;
}
