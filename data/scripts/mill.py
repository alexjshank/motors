try:
	mill_variable
except NameError:
	#initialize vars
	mill_variable = { }
	
def mill_onInit(curID):
	setModel(curID,"data/models/mill.md2","data/models/LumberMill.JPG")
	setSpeed(curID, 0, 0)
	setHealth(curID, 500)

	initEntity(curID, 6, 2)
	
def mill_onThink(curID):
	pass
	
def mill_onAttacked(curID, attackerID):
	pass
	
def mill_onSelected(curID):
	pass
	
def mill_onUnselected(curID):
	pass
