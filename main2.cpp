#include <iostream>
#include <utility>
#include <vector>
#include <unordered_set>
#include<climits>

using namespace std;

const float FLOORBOUNUS = 1;
const float BUILDINGBOUNUS = 0;
const float FLOORPUNISH = FLOORBOUNUS;
const float BUILDINGPUNISH = 0;


class team{
public:
  unsigned int capacity;
  unordered_set<int> preferred;
  vector<int> tolerated;
  unordered_set<int> noway;

public:
  team() : capacity(0) {};
  team(unsigned int capacity, unordered_set<int> preferred, vector<int> tolerated, unordered_set<int> noway){
    this->capacity=capacity;
    this->preferred=std::move(preferred);
    this->tolerated=std::move(tolerated);
    this->noway=std::move(noway);
  }
  team(int teamnum, unsigned int capacity, unordered_set<int> preferred, vector<int> tolerated, int maxteamnumber){
    this->capacity=capacity;
    unordered_set<int> used(preferred.begin(), preferred.end());
    for (int i:tolerated) used.insert(i);
    used.insert(teamnum);
    this->preferred=std::move(preferred);
    this->tolerated=std::move(tolerated);
    unordered_set<int> nowayVec;
    for (int i=1; i<=maxteamnumber; i++) {
      if (used.find(i) == used.end()) nowayVec.insert(i);
    }
    this->noway = std::move(nowayVec);
  }
};

vector<vector<vector<int>>> all_floorplans;
vector<int> og_floorCapacity;

vector<int> rankTeam(vector<team> &allTeam){
  vector<int> out(allTeam.size(), 0);
  for (int i=0; i<allTeam.size(); i++){  // loop team
    for (int j : allTeam[i].preferred){  // loop team preference
      if ((allTeam[j-1].noway).find(i+1) == (allTeam[j-1].noway).end()) {// check for noway conflict
        out[j-1]++;
      }
    }
  }
  return out;
}

void dfs(vector<team> allTeam, vector<int> floors, vector<vector<int>> floorplan, vector<bool> visited){
  // all_floorplans.push_back(floorplan);
  bool all_visited = true;
  for(auto vis : visited){
    if(!vis) all_visited = false;
  }
    //if no floor has space left for any team then add floor plan to the all floor plans
  bool flag = true;
  for(int f=0;f< floors.size(); f++){
    // cout<<"floor "<<f<<"capacity: "<< floors[f];
    if(floors[f]<0 || floors[f]>og_floorCapacity[f]/4){
//       cout<<"floor capacity neg "<<endl;
      flag = false;
      break;
    }
    for(int t=0; t< allTeam.size();t++){
      if(allTeam[t].capacity<=floors[f] && !visited[t]){
        flag = false;
//         cout<<"floor capacity exceeds team count"<<endl;
        break;
      }
    }
  }
  if(flag){
//     cout<<"********************************************************************pushing floor plan"<<endl;
    all_floorplans.push_back(floorplan);
  }
    if(all_visited) return;
  //check if valid recursion for floor plan
  // if( floors[floor_num]<0) return;
  // if(floors[floor_num]<allTeam[idx].capacity) return;

  for(int i =0;i<allTeam.size();i++){
    if(visited[i]) continue;
    visited[i] = true;

    int team_size = allTeam[i].capacity;

    int team_num = i+1;
    for(int f=0;f<floors.size();f++){
      bool noway = false;
      for(int team_n1 : floorplan[f]){
        if(allTeam[team_n1 - 1].noway.find(team_num) != allTeam[team_n1 - 1].noway.end()) noway=true;
      }
      if(noway) {
        continue;
      }
      if(floors[f]-team_size>=0){
        floors[f]-=team_size;
      } else{
        continue;
      }

      floorplan[f].push_back(team_num);
      dfs(allTeam, floors, floorplan, visited);
      floorplan[f].pop_back();
      floors[f]+=team_size;
    }
  }
}

float calScoreFloor(const vector<int>& floor, const vector<team> & teams){
  float out=0;
  for (int i : floor){
    for (int j : floor) {
      if (i!=j){
        if (teams[i-1].preferred.find(j) != teams[i-1].preferred.end()) out+=FLOORBOUNUS;
        else if (teams[i-1].noway.find(j) != teams[i-1].noway.end())  out-=FLOORPUNISH;
      }
    }
  }
  return out;
}

float calScoreBuilding(const vector<vector<int>>& groups, const vector<team> & teams){
  float out=0;
  for (int i=0; i<groups.size(); i++){  // target floor
    for(int j=0; j<groups.size(); j++){  // other floors
      if (i!=j) {
        for (int x : groups[i])  {  //target group on floors
          for (int y : groups[j]) {
            if (teams[y-1].preferred.find(x) != teams[y-1].preferred.end()) out += BUILDINGBOUNUS;
            if (teams[y-1].noway.find(x) != teams[y-1].noway.end()) out += BUILDINGPUNISH;
          }
        }
      }
    }
  }
  return out;
}

float calScorePlan(const vector<vector<int>>& groups, vector<team> & teams){
  float out = 0;
  for(const vector<int>& floor : groups) {
    float score = calScoreFloor(floor, teams);
    out += score;
  }
  out += calScoreBuilding(groups, teams);
  return out;
}

int main(){
  vector<int> floorCapacity = {43, 81, 73, 54, 97};
  og_floorCapacity = floorCapacity;
  vector<team> allTeam;
  // allTeam.push_back(team(0, 0, {0}, {0}, 0));
  allTeam.push_back(team(1, 22, {2,4,6,11}, {3,8,10}, 11));
  allTeam.push_back(team(2, 45, {1,3,5}, {6,7,11}, 11));
  allTeam.push_back(team(3, 34, {1,2,11}, {7}, 11));
  allTeam.push_back(team(4, 51, {10}, {1,3}, 11));
  allTeam.push_back(team(5, 11, {1,2,3,4}, {9,10,11}, 11));
  allTeam.push_back(team(6, 37, {7,10}, {1,8}, 11));
  allTeam.push_back(team(7, 42, {1,2,3,4,5,6}, {10,11}, 11));
  allTeam.push_back(team(8, 16, {1,10}, {2,4,11}, 11));
  allTeam.push_back(team(9, 29, {1,5}, {2,10}, 11));
  allTeam.push_back(team(10, 56, {2,6,7,11}, {4,5,8}, 11));
  allTeam.push_back(team(11, 49, {1,4,5}, {2,3,6,7,9,10}, 11));

  vector<vector<int>> floorplan(floorCapacity.size(), vector<int>());
  vector<bool> visited(allTeam.size(), false);
  dfs(allTeam, floorCapacity, floorplan, visited);

  vector<vector<int>> result;
  float max_score = 0;
  for(const auto& fl : all_floorplans){
    float score = calScorePlan(fl, allTeam);
    if(score > max_score){
      result = fl;
      max_score = score;
    }
  }
  for(int flr = 0; flr<result.size(); flr++){
    cout<<"Floor "<<flr+1<<" : ";
    for(int tm : result[flr]){
      cout<<tm<<' ';
    }
    cout<<endl;
  }
  cout << "final rating " << max_score << endl;
  return 0;
}