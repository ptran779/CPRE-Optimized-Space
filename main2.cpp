#include <iostream>
#include <utility>
#include <vector>
#include <unordered_set>

using namespace std;

class team
{
public:
  unsigned int capacity;
  vector<int> preferred;
  vector<int> tolerated;
  unordered_set<int> noway;
  int rank_score = 0;

public:
  team() : capacity(0){};
  team(unsigned int capacity, vector<int> preferred, vector<int> tolerated, unordered_set<int> noway)
  {
    this->capacity = capacity;
    this->preferred = std::move(preferred);
    this->tolerated = std::move(tolerated);
    this->noway = std::move(noway);
  }
  team(int teamnum, unsigned int capacity, vector<int> preferred, vector<int> tolerated, int maxteamnumber)
  {
    this->capacity = capacity;
    unordered_set<int> used(preferred.begin(), preferred.end());
    for (int i : tolerated)
      used.insert(i);
    used.insert(teamnum);
    this->preferred = std::move(preferred);
    this->tolerated = std::move(tolerated);
    unordered_set<int> nowayVec;
    for (int i = 1; i <= maxteamnumber; i++)
    {
      if (used.find(i) == used.end())
        nowayVec.insert(i);
    }
    this->noway = std::move(nowayVec);
  }
};

vector<int> rankTeam(vector<team> &allTeam)
{
  vector<int> out(allTeam.size(), 0);
  for (int i = 0; i < allTeam.size(); i++)
  { // loop team
    for (int j : allTeam[i].preferred)
    { // loop team preference
      if ((allTeam[j - 1].noway).find(i + 1) == (allTeam[j - 1].noway).end())
      { // check for noway conflict
        out[j - 1]++;
      }
    }
  }
  return out;
}

vector<vector<int>> solverPerFloor(unsigned int capacity, vector<team> teams)
{
  vector<vector<int>> store;
  for (int i = 0; i < teams.size(); i++)
  {
    vector<int> v = {i};
    store.push_back(v);
    cout << i << endl;
    for (int j : teams[i].preferred)
    {
      if ((teams[i].capacity + teams[j - 1].capacity) <= capacity)
      {
        bool broken = false;
        for (int k : teams[j - 1].noway)
        {
          if (k - 1 == i)
          {
            broken = true;
            break;
          }
        }
        if (broken)
          break;
        cout << i << " " << j - 1 << endl;
        vector<int> v = {i, j - 1};
        store.push_back(v);
      }
    }
    for (int j : teams[i].tolerated)
    {
      if ((teams[i].capacity + teams[j - 1].capacity) <= capacity)
      {
        bool broken = false;
        for (int l : teams[j - 1].noway)
        {
          if (l - 1 == i)
          {
            broken = true;
            break;
          }
        }
        if (broken)
          break;
        cout << i << " " << j - 1 << endl;
        vector<int> v = {i, j - 1};
        store.push_back(v);
      }
    }
  }
  return store;
}

vector<vector<vector<int>>> validFloorPlans(vector<vector<int>> FloorA, vector<vector<int>> FloorB,
                                            vector<vector<int>> FloorC, vector<vector<int>> FloorD,
                                            vector<vector<int>> FloorE)
{
  vector<vector<vector<int>>> validFloors;
  unordered_set<int> stored;
  for (int i = 0; i < FloorA.size(); i++)
  {
    for (int j = 0; j < FloorB.size(); j++)
    {
      for (int k = 0; k < FloorC.size(); k++)
      {
        for (int l = 0; l < FloorD.size(); l++)
        {
          for (int m = 0; m < FloorE.size(); m++)
          {
            stored.clear();
            if (FloorA[i].size() == 1 && (stored.find(FloorA[i][0]) != stored.end()))
              stored.insert(FloorA[i][0]);
            else if (FloorA[i].size() == 2 && (stored.find(FloorA[i][0]) != stored.end()) && (stored.find(FloorA[i][1]) != stored.end()))
            {
              stored.insert(FloorA[i][0]);
              stored.insert(FloorA[i][1]);
            }
            else
            {
              continue;
            }
            if (FloorB[j].size() == 1 && (stored.find(FloorB[j][0]) != stored.end()))
              stored.insert(FloorB[j][0]);
            else if (FloorB[j].size() == 2 && (stored.find(FloorB[j][0]) != stored.end()) && (stored.find(FloorB[j][1]) != stored.end()))
            {
              stored.insert(FloorB[j][0]);
              stored.insert(FloorB[j][1]);
            }
            else
            {
              continue;
            }
            if (FloorC[k].size() == 1 && (stored.find(FloorC[k][0]) != stored.end()))
              stored.insert(FloorC[k][0]);
            else if (FloorC[k].size() == 2 && (stored.find(FloorC[k][0]) != stored.end()) && (stored.find(FloorC[k][1]) != stored.end()))
            {
              stored.insert(FloorC[k][0]);
              stored.insert(FloorC[k][1]);
            }
            else
            {
              continue;
            }
            if (FloorD[l].size() == 1 && (stored.find(FloorD[l][0]) != stored.end()))
              stored.insert(FloorD[l][0]);
            else if (FloorD[l].size() == 2 && (stored.find(FloorD[l][0]) != stored.end()) && (stored.find(FloorD[l][1]) != stored.end()))
            {
              stored.insert(FloorD[l][0]);
              stored.insert(FloorD[l][1]);
            }
            else
            {
              continue;
            }
            if (FloorE[m].size() == 1 && (stored.find(FloorE[m][0]) != stored.end()))
              stored.insert(FloorE[m][0]);
            else if (FloorE[m].size() == 2 && (stored.find(FloorE[m][0]) != stored.end()) && (stored.find(FloorE[m][1]) != stored.end()))
            {
              stored.insert(FloorE[m][0]);
              stored.insert(FloorE[m][1]);
            }
            else
            {
              continue;
            }
            validFloors.push_back({FloorA[i], FloorB[j], FloorC[k], FloorD[l], FloorE[m]});
          }
        }
      }
    }
  }
  return validFloors;
}

int main()
{
  vector<int> floorCapacity = {43, 81, 73, 54, 97};
  vector<team> allTeam;
  allTeam.push_back(team(22, {2, 4, 6, 11}, {3, 8, 10}, {5, 7, 9}));
  allTeam.push_back(team(45, {1, 3, 5}, {6, 7, 11}, {4, 8, 9, 10}));
  allTeam.push_back(team(34, {1, 2, 11}, {7}, {4, 5, 6, 8, 9, 10}));
  allTeam.push_back(team(51, {10}, {1, 3}, {2, 5, 6, 7, 8, 9, 11}));
  allTeam.push_back(team(11, {1, 2, 3, 4}, {9, 10, 11}, {6, 7, 8}));
  allTeam.push_back(team(37, {7, 10}, {1, 8}, {2, 3, 4, 5, 9, 11}));
  allTeam.push_back(team(42, {1, 2, 3, 4, 5, 6}, {10, 11}, {8, 9}));
  allTeam.push_back(team(16, {1, 10}, {2, 4, 11}, {3, 5, 6, 7, 9}));
  allTeam.push_back(team(29, {1, 5}, {2, 10}, {3, 4, 6, 7, 8, 11}));
  allTeam.push_back(team(56, {2, 6, 7, 11}, {4, 5, 8}, {1, 3, 9}));
  allTeam.push_back(team(49, {1, 4, 5}, {2, 3, 6, 7, 9, 10}, {8}));

  cout << "hello uhack" << endl;
  cout << "hello uhack 2" << endl;
  // floor A
  // for (team i : allTeam)
  // {
  //   for (int j : *i.noway)
  //     cout << j << " ";
  //   cout << endl;
  // }

  // valid floors for A
  vector<vector<int>> FloorA = solverPerFloor(97, allTeam);
  for (int i = 0; i < FloorA.size(); i++)
  {
    for (int j : FloorA[i])
    {
      cout << FloorA[i][j] + " ";
    }
    cout << endl;
  }
  return 0;
}