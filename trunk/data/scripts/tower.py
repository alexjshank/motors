try:
	tower_lastSpawnTime
except NameError:
	#initialize vars
	tower_lastSpawnTime = { }
	
def tower_onInit(curID):
	setModel(curID,"data/models/tower.md2","data/models/tower.JPG")
	setSpeed(curID, 0, 0)
	
def tower_onThink(curID):
	pass
	
def tower_onAttacked(curID, attackerID):
	pass
	
def tower_onSelected(curID):
	pass
	
def tower_onUnselected(curID):
	pass
