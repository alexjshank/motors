try : 
	peasant_state
except NameError :
	peasant_state = { }

def peasant_onInit(curID):
	peasant_state[curID] = 0

	setModel(curID,"data/models/peasant.md2","data/models/peasant.JPG")
	loadMenu(curID,"data/UI/peasant.ui")

def peasant_onThink(curID):
	pass
 
def peasant_onAttacked(curID, attackerID):
	echo("onattacked()")
	
def peasant_onDeath(curID):
	echo("death")
	
def peasant_onSelected(curID):
	showMenu(curID)
	
def peasant_onUnselected(curID):
	hideMenu(curID)
