try:
	farm_lastSpawnTime
except NameError:
	#initialize vars
	farm_lastSpawnTime = { }
	
def farm_onInit(curID):
	loadMenu(curID,'data/ui/farm.ui')
	setModel(curID,"data/models/farm.md2","data/models/house.JPG")
	farm_lastSpawnTime[curID] = getTime()
	setSpeed(curID, 0, 0)
	
def farm_onThink(curID):
	if getTime() - farm_lastSpawnTime[curID] > 20:
		spawnunit('sheep',getXpos(curID)+2, getZpos(curID)+2)
		farm_lastSpawnTime[curID] = getTime()
	
def farm_onAttacked(curID, attackerID):

	pass
	
def farm_onSelected(curID):
	showMenu(curID)
	
def farm_onUnselected(curID):
	hideMenu(curID)
