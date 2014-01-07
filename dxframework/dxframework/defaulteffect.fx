struct Light
{
	int type;
	float range;
	float3 position;
	float3 direction;
	float3 attenuation;
};


cbuffer perFrame
{
	float3 camPos;
};

cbuffer perObject
{
	float4 ambient;
	float4 diffuse;
	float4 emissive;
	float4 specular;
	float specularPower;
	
	float4x4 worldMat;
	float4x4 WITmat;
	float4x4 WVPmat;
	texture tex;
	
	Light light;
	
	int lighttype;
	float lightrange;
	float3 lightpos;
	float3 lightdir;
	float3 lightatt;
	float3 lightamb;

	float TexUScale = 1.0f;
	float TexVScale = 1.0f;
};

sampler texSamp = sampler_state
{
	Texture = <tex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct VertexInput
{
	float3 pos		: POSITION0;
	float3 norm		: NORMAL0;
	float2 texCoord	: TEXCOORD0;
};

struct PixelInput
{
	float4 PosH		: POSITION0;
	float3 PosW		: TEXCOORD0;
	float3 norm		: TEXCOORD1;
	float2 texCoord	: TEXCOORD2;
};

PixelInput VS( VertexInput input )
{
	PixelInput output = (PixelInput)0;
	
	output.PosH = mul( float4(input.pos, 1.0f), WVPmat );
	output.PosW = mul( float4(input.pos, 1.0f), worldMat ).xyz;
	output.norm = mul( float4(input.norm, 1.0f), WITmat ).xyz;
	
	output.norm = normalize( output.norm );
	
	output.texCoord.x = input.texCoord.x * TexUScale;
	output.texCoord.y = input.texCoord.y * TexVScale;
	
	return output;
}

float4 PSunlit( PixelInput input ) : COLOR
{
	float3 texColor = tex2D( texSamp, input.texCoord ).rgb;
	
	float3 d = (diffuse * texColor).rgb;
	//float3 d = texColor.rgb;
	
	return float4( d, 1.0f );
}

float4 PSlit( PixelInput input ) : COLOR
{
	input.norm = normalize( input.norm );
	float4 texCol = tex2D( texSamp, input.texCoord );
	float3 toEye = normalize( camPos - input.PosW );
	//float3 lightDir = normalize( light.position - input.PosW );
	float3 lightDir = float3( 0.0f, 0.0f, 0.0f );
	float i = 0.0f;
	float d = 0.0f;
	float A = 0.0f;
	float4 ambient = float4( 0.0, 0.0, 0.0, 0.0 );
	float4 color = float4( 0.0, 0.0, 0.0, 0.0 );
	
	float distratio = 0.0f;
	
	if( lighttype == 0 )	// point light
	{
		lightDir = normalize( lightpos - input.PosW );

		i = max( dot( lightDir, input.norm ), 0.0f );
		d = distance( lightpos, input.PosW );
		distratio = lightrange / d;
		//d = distance( lightpos, input.PosW );
		//A = lightatt[0] + lightatt[1]*d + lightatt[2]*d*d;
		
		//ambient = float4(lightamb, 1.0) * texCol;
		//color = ambient + (texCol * i)/A;
		color = texCol;
		
		if( i > 0.8f && distratio > 0.9f )
		{
			color *= float4(1.0, 1.0, 1.0, 1.0);
		}else if( i > 0.4f && distratio > 0.5f )
		{
			color *= float4(0.7, 0.7, 0.7, 1.0);
		}else if( i > 0.2f && distratio > 0.3f )
		{
			color *= float4(0.3, 0.3, 0.3, 1.0);
		}else
		{
			color *= float4( lightamb, 1.0f );
		}
	
		return color;
	}else if( lighttype == 1 )	// directional light
	{
		lightDir = float3( -lightdir.x, -lightdir.y, -lightdir.z );
		i = max( dot( lightDir, input.norm ), 0.0f );
	
		//ambient = float4(lightamb, 1.0) * texCol;
		//color = ambient + (texCol * i);
		color = texCol;
		
		if( i > 0.9f )
		{
			color *= float4( 1.0f, 1.0f, 1.0f, 1.0f );
		}else if( i > 0.5f )
		{
			color *= float4( 0.7f, 0.7f, 0.7f, 1.0f );
		}else if( i > 0.3f )
		{
			color *= float4( 0.3f, 0.3f, 0.3f, 1.0f );
		}else
		{
			color *= float4( lightamb, 1.0f );
		}
	
		return color;
	}else
	{
		float3 tCol = tex2D( texSamp, input.texCoord ).rgb;
		float3 Col = (diffuse * tCol).rgb;
		return float4( Col, 1.0f );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
//	for drawing solid color primitives
///////////////////////////////////////////////////////////////////////////////////////////
PixelInput primitiveVS( float3 pos : POSITION0 )
{
	PixelInput output = (PixelInput)0;
	output.PosH = mul( float4(pos, 1.0f), WVPmat );
	output.PosW = mul( float4(pos, 1.0f), worldMat ).xyz;
	
	return output;
}

float4 primitivePS( PixelInput input ) : COLOR
{
	float4 color = float4( 1.0f, 1.0f, 1.0f, 1.0f );
	return color;
}

/////////////////////////////////////////////////////////////////////////////////////////
//	Techniques
/////////////////////////////////////////////////////////////////////////////////////////

technique unlit
{
	pass p0
	{
		VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_3_0 PSunlit();
	}
}

technique lit
{
	pass p0
	{
		VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_3_0 PSlit();
	}
}

technique primitiveSolid
{
	pass p0
	{
		VertexShader = compile vs_3_0 primitiveVS();
		PixelShader = compile ps_3_0 primitivePS();
		
		Fillmode = Solid;
	}
}

technique primitiveWireframe
{
	pass p0
	{
		VertexShader = compile vs_3_0 primitiveVS();
		PixelShader = compile ps_3_0 primitivePS();
		
		Fillmode = Wireframe;
	}
}