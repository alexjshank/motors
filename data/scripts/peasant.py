try : 
	peasant_state
except NameError :
	peasant_state = { }
	peasant_building = { }

def peasant_onInit(curID):
	peasant_state[curID] = 0
	peasant_building[curID] = 0

	setTeam(curID,1)

	setHealth(curID, 100)
	setModel(curID,"data/models/peasant.md2","data/models/peasant.JPG")
	loadMenu(curID,"data/UI/peasant.ui")

def peasant_onThink(curID):
	if peasant_state[curID] == 0:
		
		pass
	
	if peasant_state[curID] == 1: 
		if distance(curID, peasant_building[curID]) < 4:
			setCompleted(peasant_building[curID], getCompleted(peasant_building[curID])+5)
			setSubtitle(peasant_building[curID], "%d%%"%getCompleted(peasant_building[curID]))
			
			if getCompleted(peasant_building[curID]) >= 100:
				setCompleted(peasant_building[curID],100)
				peasant_building[curID] = 0
				peasant_state[curID] = 0
 
def peasant_onAttacked(curID, attackerID):
	echo("onattacked()")
	
def peasant_onDeath(curID):
	echo("death")
	
def peasant_onSelected(curID):
	showMenu(curID)
	
def peasant_onUnselected(curID):
	hideMenu(curID)
	
def peasant_Build(curID,buildType):
	peasant_building[curID] = spawnunit(buildType,getXpos(curID),getZpos(curID))
	setCompleted(peasant_building[curID], 0)
	placeent(peasant_building[curID],"peasant_onBuild(%d,%d)"%(curID,peasant_building[curID]))
	
def peasant_onBuild(curID, buildingID):
	peasant_building[curID] = buildingID
	peasant_state[curID] = 1
	pathToEnt(curID, buildingID)