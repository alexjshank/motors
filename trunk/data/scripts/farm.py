try:
	farm_lastSpawnTime
except NameError:
	#initialize vars
	farm_lastSpawnTime = { }
	
def farm_onInit(curID):

	loadMenu(curID,'data/ui/farm.ui')
	setModel(curID,"data/models/farm.md2","data/models/house.JPG")
	farm_lastSpawnTime[curID] = getTime() - 20
	setSpeed(curID, 0, 0)
	setHealth(curID, 500)
	initEntity(curID, 2, 2)
	
def farm_onThink(curID):
	if getCompleted(curID) == 100:
		if getTime() - farm_lastSpawnTime[curID] > 20:
			setCompleted(spawnunit('sheep',getXpos(curID)+2, getZpos(curID)+2),100)
			farm_lastSpawnTime[curID] = getTime()
	
def farm_onAttacked(curID, attackerID):

	pass
	
def farm_onSelected(curID):
	showMenu(curID)
	
def farm_onUnselected(curID):
	hideMenu(curID)
