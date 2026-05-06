class CfgPatches
{
	class MoreDoors_Sounds
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data"
		};
	};
};
class CfgSoundShaders
{
	class base_SoundShaderFlies
	{
		range=20;
		rangeCurve="defaultLFECurve";
		volume=3;
	};
	class MoreDoor_doorMetalSlideLargeOpen_SoundShader: base_SoundShaderFlies
	{
		samples[]=
		{
			
			{
				"\DZ\sounds\environment\buildings\doors\MetalSlideLarge\doorMetalSlideLargeOpen_1",
				1
			},
			
			{
				"\DZ\sounds\environment\buildings\doors\MetalSlideLarge\doorMetalSlideLargeOpen_2",
				1
			},
			
			{
				"\DZ\sounds\environment\buildings\doors\MetalSlideLarge\doorMetalSlideLargeOpen_3",
				1
			},
			
			{
				"\DZ\sounds\environment\buildings\doors\MetalSlideLarge\doorMetalSlideLargeOpen_4",
				1
			}
		};
	};
	class MoreDoor_doorMetalSlideLargeClose_SoundShader: base_SoundShaderFlies
	{
		samples[]=
		{
			
			{
				"\DZ\sounds\environment\buildings\doors\MetalSlideLarge\doorMetalSlideLargeClose_1",
				1
			},
			
			{
				"\DZ\sounds\environment\buildings\doors\MetalSlideLarge\doorMetalSlideLargeClose_2",
				1
			},
			
			{
				"\DZ\sounds\environment\buildings\doors\MetalSlideLarge\doorMetalSlideLargeClose_3",
				1
			},
			
			{
				"\DZ\sounds\environment\buildings\doors\MetalSlideLarge\doorMetalSlideLargeClose_4",
				1
			}
		};
	};
	class MoreDoor_doorWoodGateSmallOpen_SoundShader: base_SoundShaderFlies
	{
		samples[]=
		{
			
			{
				"\DZ\sounds\environment\buildings\doors\WoodGateSmall\doorWoodGateSmallOpen_1",
				1
			},
			
			{
				"\DZ\sounds\environment\buildings\doors\WoodGateSmall\doorWoodGateSmallOpen_2",
				1
			}
		};
	};
	class MoreDoor_doorWoodGateSmallClose_SoundShader: base_SoundShaderFlies
	{
		samples[]=
		{
			
			{
				"\DZ\sounds\environment\buildings\doors\WoodGateSmall\doorWoodGateSmallClose_1",
				1
			},
			
			{
				"\DZ\sounds\environment\buildings\doors\WoodGateSmall\doorWoodGateSmallClose_2",
				1
			}
		};
	};
	class MoreDoor_doorMetalGateMeshOpen_SoundShader: base_SoundShaderFlies
	{
		samples[]=
		{
			
			{
				"\DZ\sounds\environment\buildings\doors\MetalGateMesh\doorMetalGateMeshOpen_1",
				1
			},
			
			{
				"\DZ\sounds\environment\buildings\doors\MetalGateMesh\doorMetalGateMeshOpen_1",
				1
			}
		};
	};
	class MoreDoor_doorMetalGateMeshClose_SoundShader: base_SoundShaderFlies
	{
		samples[]=
		{
			
			{
				"\DZ\sounds\environment\buildings\doors\MetalGateMesh\doorMetalGateMeshClose_1",
				1
			},
			
			{
				"\DZ\sounds\environment\buildings\doors\MetalGateMesh\doorMetalGateMeshClose_1",
				1
			}
		};
	};
	class MoreDoor_doorMetalGateOpen_SoundShader: base_SoundShaderFlies
	{
		samples[]=
		{
			
			{
				"\DZ\sounds\environment\buildings\doors\MetalGate\doorMetalGateOpen_1",
				1
			},
			
			{
				"\DZ\sounds\environment\buildings\doors\MetalGate\doorMetalGateOpen_2",
				1
			}
		};
	};
	class MoreDoor_doorMetalGateClose_SoundShader: base_SoundShaderFlies
	{
		samples[]=
		{
			
			{
				"\DZ\sounds\environment\buildings\doors\MetalGate\doorMetalGateClose_1",
				1
			},
			
			{
				"\DZ\sounds\environment\buildings\doors\MetalGate\doorMetalGateClose_2",
				1
			}
		};
	};
	class MoreDoor_doorWoodSmallOpen_SoundShader: base_SoundShaderFlies
	{
		samples[]=
		{
			
			{
				"\DZ\sounds\environment\buildings\doors\WoodSmall\doorWoodSmallOpen_1",
				1
			},
			
			{
				"\DZ\sounds\environment\buildings\doors\WoodSmall\doorWoodSmallOpen_2",
				1
			},
			
			{
				"\DZ\sounds\environment\buildings\doors\WoodSmall\doorWoodSmallOpen_3",
				1
			},
			
			{
				"\DZ\sounds\environment\buildings\doors\WoodSmall\doorWoodSmallOpen_4",
				1
			}
		};
	};
	class MoreDoor_doorWoodSmallClose_SoundShader: base_SoundShaderFlies
	{
		samples[]=
		{
			
			{
				"\DZ\sounds\environment\buildings\doors\WoodSmall\doorWoodSmallClose_1",
				1
			},
			
			{
				"\DZ\sounds\environment\buildings\doors\WoodSmall\doorWoodSmallClose_2",
				1
			},
			
			{
				"\DZ\sounds\environment\buildings\doors\WoodSmall\doorWoodSmallClose_3",
				1
			},
			
			{
				"\DZ\sounds\environment\buildings\doors\WoodSmall\doorWoodSmallClose_4",
				1
			},
			
			{
				"\DZ\sounds\environment\buildings\doors\WoodSmall\doorWoodSmallClose_5",
				1
			},
			
			{
				"\DZ\sounds\environment\buildings\doors\WoodSmall\doorWoodSmallClose_6",
				1
			},
			
			{
				"\DZ\sounds\environment\buildings\doors\WoodSmall\doorWoodSmallClose_7",
				1
			}
		};
	};
	class MoreDoor_doorWoodGreenhouseOpen_SoundShader: base_SoundShaderFlies
	{
		samples[]=
		{
			
			{
				"\DZ\sounds\environment\buildings\doors\WoodGreenhouse\doorWoodGreenhouseOpen_1",
				1
			},
			
			{
				"\DZ\sounds\environment\buildings\doors\WoodGreenhouse\doorWoodGreenhouseOpen_2",
				1
			}
		};
	};
	class MoreDoor_doorWoodGreenhouseClose_SoundShader: base_SoundShaderFlies
	{
		samples[]=
		{
			
			{
				"\DZ\sounds\environment\buildings\doors\WoodGreenhouse\doorWoodGreenhouseClose_1",
				1
			},
			
			{
				"\DZ\sounds\environment\buildings\doors\WoodGreenhouse\doorWoodGreenhouseClose_2",
				1
			}
		};
	};
	class MoreDoor_doorMetalSmallOpen_SoundShader: base_SoundShaderFlies
	{
		samples[]=
		{
			
			{
				"\DZ\sounds\environment\buildings\doors\MetalSmall\doorMetalSmallOpen_1",
				1
			}
		};
	};
	class MoreDoor_doorMetalSmallClose_SoundShader: base_SoundShaderFlies
	{
		samples[]=
		{
			
			{
				"\DZ\sounds\environment\buildings\doors\MetalSmall\doorMetalSmallClose_1",
				1
			}
		};
	};
	class MoreDoor_doorWoodNolatchOpen_SoundShader: base_SoundShaderFlies
	{
		samples[]=
		{
			
			{
				"\DZ\sounds\environment\buildings\doors\WoodNolatch\doorWoodNolatchOpen_1",
				1
			},
			
			{
				"\DZ\sounds\environment\buildings\doors\WoodNolatch\doorWoodNolatchOpen_2",
				1
			},
			
			{
				"\DZ\sounds\environment\buildings\doors\WoodNolatch\doorWoodNolatchOpen_3",
				1
			},
			
			{
				"\DZ\sounds\environment\buildings\doors\WoodNolatch\doorWoodNolatchOpen_4",
				1
			}
		};
	};
	class MoreDoor_doorWoodNolatchClose_SoundShader: base_SoundShaderFlies
	{
		samples[]=
		{
			
			{
				"\DZ\sounds\environment\buildings\doors\WoodNolatch\doorWoodNolatchClose_1",
				1
			},
			
			{
				"\DZ\sounds\environment\buildings\doors\WoodNolatch\doorWoodNolatchClose_2",
				1
			},
			
			{
				"\DZ\sounds\environment\buildings\doors\WoodNolatch\doorWoodNolatchClose_3",
				1
			},
			
			{
				"\DZ\sounds\environment\buildings\doors\WoodNolatch\doorWoodNolatchClose_4",
				1
			}
		};
	};
};
class CfgSoundSets
{
	class base_SoundSet
	{
		sound3DProcessingType="defaultLFECurve";
		spatial=1;
		doppler=0;
		loop=0;
		volume=30;
	};
	class base_SoundSetLoop
	{
		sound3DProcessingType="defaultLFECurve";
		spatial=1;
		doppler=0;
		loop=1;
	};
	class MoreDoor_doorMetalSlideLargeOpen_SoundSet: base_SoundSet
	{
		soundShaders[]=
		{
			"MoreDoor_doorMetalSlideLargeOpen_SoundShader"
		};
	};
	class MoreDoor_doorMetalSlideLargeClose_SoundSet: base_SoundSet
	{
		soundShaders[]=
		{
			"MoreDoor_doorMetalSlideLargeClose_SoundShader"
		};
	};
	class MoreDoor_doorWoodGateSmallOpen_SoundSet: base_SoundSet
	{
		soundShaders[]=
		{
			"MoreDoor_doorWoodGateSmallOpen_SoundShader"
		};
	};
	class MoreDoor_doorWoodGateSmallClose_SoundSet: base_SoundSet
	{
		soundShaders[]=
		{
			"MoreDoor_doorWoodGateSmallClose_SoundShader"
		};
	};
	class MoreDoor_doorMetalGateMeshOpen_SoundSet: base_SoundSet
	{
		soundShaders[]=
		{
			"MoreDoor_doorMetalGateMeshOpen_SoundShader"
		};
	};
	class MoreDoor_doorMetalGateMeshClose_SoundSet: base_SoundSet
	{
		soundShaders[]=
		{
			"MoreDoor_doorMetalGateMeshClose_SoundShader"
		};
	};
	class MoreDoor_doorMetalGateOpen_SoundSet: base_SoundSet
	{
		soundShaders[]=
		{
			"MoreDoor_doorMetalGateOpen_SoundShader"
		};
	};
	class MoreDoor_doorMetalGateClose_SoundSet: base_SoundSet
	{
		soundShaders[]=
		{
			"MoreDoor_doorMetalGateClose_SoundShader"
		};
	};
	class MoreDoor_doorWoodSmallOpen_SoundSet: base_SoundSet
	{
		soundShaders[]=
		{
			"MoreDoor_doorWoodSmallOpen_SoundShader"
		};
	};
	class MoreDoor_doorWoodSmallClose_SoundSet: base_SoundSet
	{
		soundShaders[]=
		{
			"MoreDoor_doorWoodSmallClose_SoundShader"
		};
	};
	class MoreDoor_doorWoodGreenhouseOpen_SoundSet: base_SoundSet
	{
		soundShaders[]=
		{
			"MoreDoor_doorWoodGreenhouseOpen_SoundShader"
		};
	};
	class MoreDoor_doorWoodGreenhouseClose_SoundSet: base_SoundSet
	{
		soundShaders[]=
		{
			"MoreDoor_doorWoodGreenhouseClose_SoundShader"
		};
	};
	class MoreDoor_doorMetalSmallOpen_SoundSet: base_SoundSet
	{
		soundShaders[]=
		{
			"MoreDoor_doorMetalSmallOpen_SoundShader"
		};
	};
	class MoreDoor_doorMetalSmallClose_SoundSet: base_SoundSet
	{
		soundShaders[]=
		{
			"MoreDoor_doorMetalSmallClose_SoundShader"
		};
	};
	class MoreDoor_doorWoodNolatchOpen_SoundSet: base_SoundSet
	{
		soundShaders[]=
		{
			"MoreDoor_doorWoodNolatchOpen_SoundShader"
		};
	};
	class MoreDoor_doorWoodNolatchClose_SoundSet: base_SoundSet
	{
		soundShaders[]=
		{
			"MoreDoor_doorWoodNolatchClose_SoundShader"
		};
	};
};
