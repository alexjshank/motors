try:
	barracks_lastSpawnTime
except NameError:
	#initialize vars
	barracks_lastSpawnTime = { }
	
def barracks_onInit(curID):
	loadMenu(curID,'data/ui/barracks.ui')
	setModel(curID,"data/models/barracks.md2","data/models/barracks.JPG")
	setSpeed(curID, 0, 0)
	setHealth(curID, 1000)

	initEntity(curID, 3, 2)
	
def barracks_onThink(curID):
	pass
	
def barracks_onAttacked(curID, attackerID):
	pass
	
def barracks_onSelected(curID):
	showMenu(curID)
	
def barracks_onUnselected(curID):
	hideMenu(curID)
