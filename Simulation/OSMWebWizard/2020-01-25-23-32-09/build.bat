python "%SUMO_HOME%\tools\randomTrips.py" -n osm.net.xml --seed 42 --fringe-factor 2 -p 19.387190 -o osm.bicycle.trips.xml -e 3600 --vehicle-class bicycle --vclass bicycle --prefix bike --fringe-start-attributes "departSpeed=\"max\"" --max-distance 8000 --trip-attributes "departLane=\"best\"" --validate
python "%SUMO_HOME%\tools\randomTrips.py" -n osm.net.xml --seed 42 --fringe-factor 2 -p 38.082052 -o osm.motorcycle.trips.xml -e 3600 --vehicle-class motorcycle --vclass motorcycle --prefix moto --fringe-start-attributes "departSpeed=\"max\"" --max-distance 1200 --trip-attributes "departLane=\"best\"" --validate
python "%SUMO_HOME%\tools\randomTrips.py" -n osm.net.xml --seed 42 --fringe-factor 5 -p 12.694017 -o osm.passenger.trips.xml -e 3600 --vehicle-class passenger --vclass passenger --prefix veh --min-distance 300 --trip-attributes "departLane=\"best\"" --fringe-start-attributes "departSpeed=\"max\"" --allow-fringe.min-length 1000 --lanes --validate
python "%SUMO_HOME%\tools\randomTrips.py" -n osm.net.xml --seed 42 --fringe-factor 40 -p 11.159925 -o osm.ship.trips.xml -e 3600 --vehicle-class ship --vclass ship --prefix ship --validate --fringe-start-attributes "departSpeed=\"max\""
python "%SUMO_HOME%\tools\randomTrips.py" -n osm.net.xml --seed 42 --fringe-factor 40 -p 292.273744 -o osm.rail_urban.trips.xml -e 3600 --vehicle-class rail_urban --vclass rail_urban --prefix urban --fringe-start-attributes "departSpeed=\"max\"" --min-distance 1800 --trip-attributes "departLane=\"best\"" --validate
python "%SUMO_HOME%\tools\randomTrips.py" -n osm.net.xml --seed 42 --fringe-factor 40 -p 555.349870 -o osm.rail.trips.xml -e 3600 --vehicle-class rail --vclass rail --prefix rail --fringe-start-attributes "departSpeed=\"max\"" --min-distance 2400 --trip-attributes "departLane=\"best\"" --validate
