//MODIFICAÇÃO DO MARTELÃO PARA ADICIONAR AÇÃO DE QUEBRAR PEDRAS
modded class SledgeHammer
{
	override void SetActions()
	{
		super.SetActions();

		AddAction(ActionBrokeStone);
		//AddAction(ActionDestroyParts);
	}
};
//MODIFICAÇÃO DA CLASSE DA PEDRA DO MAPA
modded class RockBase
{
	//O QUE VAI GERAR APOS FAZER A ANIMAÇÃO COMPLETA DE MINERAR
	override void GetMaterialAndQuantityMap(ItemBase item, out map<string,int> output_map)
	{
		if ( item && item.KindOf("Pickaxe") )
		{
			output_map.Insert("PB_Stone",1);
			output_map.Insert("Stone",1);
			output_map.Insert("SmallStone",1);
		}
		else if ( item && item.KindOf("SledgeHammer") )
		{
			output_map.Insert("Stone",1);
			output_map.Insert("SmallStone",1);
			output_map.Insert("SmallStone",1);
		}
		else if ( item )
		{
			output_map.Insert("Stone",1);
		}
	}
};
//CLASSE DA PEDRA GRANDE
class PB_Stone extends Inventory_Base
{
	override bool CanPutInCargo( EntityAI parent )
    {
        return false;
    }

	override bool CanPutIntoHands(EntityAI parent)
    {
        return true;
    }
};
class PB_Brick extends Inventory_Base
{
	override bool CanPutInCargo( EntityAI parent )
    {
        return true;
    }

	override bool CanPutIntoHands(EntityAI parent)
    {
        return true;
    }
};
