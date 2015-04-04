## General ##
  * selecting entities in edit mode should bring up a tool window to allow the user to modify settings about the entity
  * Butchers shop
  * [FoodHarvester](FoodHarvester.md) should return sheeps to the butchers shop instead of the lumber yard
  * resources should be able to be 'exported' from each lumber mill into a unit-carriable form (to be transportable on ships to new build locations)
  * should be an option on [Towers](Towers.md) to build a [Wall](Wall.md) between two towers
  * [Towers](Towers.md) and [Wall](Wall.md)s should be able to hold units
  * [GrainSilos](GrainSilos.md)
  * [GrainField](GrainField.md)s (harvested by peasants every 2 minutes or so, stored in the grain silos)



## Ships ##
  * [Ships](Ships.md) should be able to carry units and resources, and drop them off somewhere else
  * ships should be able to fire their cannons


## Scripting ##
  * need to make "QueueUnitForBuild" function to enable queuing of units to be built (to replace Spawn('...') build buttons)


## AIPlayer ##
  * controls an enemy team in the same manner the user controls their team


## Sound System ##
  * integrate openal
  * need sounds: enemy unit spotted, our units are under attack, unit attacking, unit confirm order, unit deaths, building deaths, unit spawned, building contruction complete


## UI ##
  * a [?] help button in the top right corner that pops down a controls cheat-sheet.
  * textedit boxes
  * drop-down list boxes
  * check boxes
  * should have in-game configuration editor


## Building animations ##
  * when a building is first placed, its "construction completed" percent should be 0. and the building model shouldn't be rendered.
  * the building render function should render a box with an "under construction" texture on it. which after completing construction, rolls away as its alpha fades away into invisibility.

## Models needed ##
  * butcher shop
  * grain silo
  * lots of different types of ships/boats
  * stone wall segment
  * lower poly tree
  * rocks


## Environment ##
  * cloud system
  * clouds should be able to become a storm, and drop rain/lightning
  * day/night cycles