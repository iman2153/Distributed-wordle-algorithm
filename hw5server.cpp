#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/value.h>
#include "hw5server.h"
#include "hw5client.h"
#include <jsonrpccpp/server/connectors/httpserver.h>
#include "person.h"
#include "thing.h"
#include "gps.h"
#include "time.h"

using namespace jsonrpc;
using namespace std;

class Myhw5Server : public hw5Server
{
public:
  std::vector<person> persons;
  std::vector<thing> things;
  _time the_time;
  gps location;
  Myhw5Server(const Json::Value &info);
  virtual bool Json2Object(const Json::Value &info);
  virtual ~Myhw5Server() {};
  virtual Json::Value dump2JSON()const;
  virtual Json::Value gainPerson(person t);
  virtual Json::Value gainThing(thing t);
  virtual Json::Value losePerson(string name);
  virtual Json::Value loseThing(string name);
  virtual Json::Value findPerson(string name);
  virtual Json::Value findThing(string name);
  Myhw5Server(AbstractServerConnector &connector, serverVersion_t type);
  virtual Json::Value action(const Json::Value& info);
};

Myhw5Server::Myhw5Server(AbstractServerConnector &connector, serverVersion_t type) : hw5Server(connector, type) {}

Json::Value Myhw5Server::action(const Json::Value& info) {
  if(info["Type"] == "transfer_thing") {
    for(int i = 0; i < persons.size(); i++) {
      if(info["from_person"].asString() != persons[i].name) continue;
      for(int j = 0; j < persons.size(); j++) {
        if(info["to_person"].asString() != persons[j].name) continue;
        persons[i].loseThing(info["thing"]["name"].asString());
        persons[j].gainThing(thing(info["thing"]));
        return dump2JSON();
      }
    }
  }
  if(info["Type"] == "move_from") {
    Json::Value result;
    Json::Value objs;
    if(info["persons"]) {
      Json::Value list;
      for(int i = 0; i < info["persons"].size(); i++) {
        list[i] = losePerson(info["persons"][i].asString());
      }
      objs["persons"] = list;
    }
    if(info["things"]) {
      Json::Value list;
      for(int i = 0; i < info["things"].size(); i++) {
        list[i] = loseThing(info["things"][i].asString());
      }
      objs["things"] = list;
    }
    result["objs"] = objs;
    return result;
  }
  if(info["Type"] == "move_to") {
    Json2Object(info["objs"]);
  }
  if(info["Type"] == "update") {
    Json2Object(info);
  }
  return dump2JSON();
}
bool Myhw5Server::Json2Object(const Json::Value &info) {
  if(!info) return false;
  if(info["persons"]) {
      for(int i = 0; i < info["persons"].size(); i++) {
          gainPerson(person(info["persons"][i]));
      }
  }
  if(info["things"]) {
      for(int i = 0; i < info["things"].size(); i++) {
          gainThing(thing(info["things"][i]));
      }
  }
  if(info["time"]) {
    the_time = _time(info["time"]);
  }
  if(info["location"]) {
    location = gps(info["location"]);
  }
  return true;
}
Json::Value Myhw5Server::dump2JSON()const {
  Json::Value v;
  Json::Value persons_list;
  for(int i = 0; i < persons.size(); i++) {
      persons_list[i] = persons[i].dump2JSON();
  }
  v["persons"] = persons_list;
  Json::Value things_list;
  for(int i = 0; i < things.size(); i++) {
      things_list[i] = things[i].dump2JSON();
  }
  v["things"] = things_list;
  v["time"] = the_time.dump2JSON();
  v["location"] = location.dump2JSON();
  return v;
}
Json::Value Myhw5Server::gainPerson(person p) {
  persons.push_back(p);
  for(int i = 0; i < p.inventory.size(); i++) {
    gainThing(p.inventory[i]);
  }
  Json::Value v;
  v["person"] = p.dump2JSON();
  return v;
}
Json::Value Myhw5Server::gainThing(thing t) {
  things.push_back(t);
  Json::Value v;
  v["thing"] = t.dump2JSON();
  return v;
}
Json::Value Myhw5Server::losePerson(string name) {
  Json::Value result;
  for(int i = 0; i < persons.size(); i++) {
    if(persons[i].name != name) continue;
    for(int j = 0; j < persons[i].inventory.size(); j++) {
      loseThing(persons[i].inventory[j].name);
    }
    result = persons[i].dump2JSON();
    persons.erase(persons.begin() + i);
    break;
  }
  return result;
}
Json::Value Myhw5Server::loseThing(string name) {
  Json::Value result;
  for(int i = 0; i < things.size(); i++) {
    if(things[i].name != name) continue;
    result = things[i].dump2JSON();
    things.erase(things.begin() + i);
    break;
  }
  return result;
}
Json::Value Myhw5Server::findPerson(string name) {
  for(int i = 0; i < persons.size(); i++) {
    if(persons[i].name != name) continue;
    return persons[i].dump2JSON();
  }
  Json::Value not_found;
  return not_found;
}
Json::Value Myhw5Server::findThing(string name) {
  for(int i = 0; i < things.size(); i++) {
    if(things[i].name != name) continue;
    return things[i].dump2JSON();
  }
  Json::Value not_found;
  return not_found;
}

int main() {
  HttpServer httpserver1(8001);
  Myhw5Server hw5Home(httpserver1, JSONRPC_SERVER_V1V2);
  HttpServer httpserver2(8002);
  Myhw5Server hw5Forest(httpserver2, JSONRPC_SERVER_V1V2);
  HttpServer httpserver3(8003);
  Myhw5Server hw5GrandmaHome(httpserver3, JSONRPC_SERVER_V1V2);

  hw5Home.StartListening();
  hw5Forest.StartListening();
  hw5GrandmaHome.StartListening();

  std::cout << "Hit enter to stop the server" << endl;
  getchar();

  hw5Home.StopListening();
  hw5Forest.StopListening();
  hw5GrandmaHome.StopListening();

  return 0;
}