#include <iostream>
#include <random>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

#define int long long

const int PRIME = 9223372036854775783;

struct Record{
    int identifier;
    string mail;
    string name;
    string sex;
};

static std::mt19937_64 rngEngine(std::random_device{}());

struct UniversalHash{
    int coeffA;
    int coeffB;

    UniversalHash(){
        std::uniform_int_distribution<long long> firstDistribution(1, PRIME - 1);
        std::uniform_int_distribution<long long> secondDistribution(0, PRIME - 1);

        coeffA = firstDistribution(rngEngine);
        coeffB = secondDistribution(rngEngine);
    }

    void regenerate(){
        std::uniform_int_distribution<long long> firstDistribution(1, PRIME - 1);
        std::uniform_int_distribution<long long> secondDistribution(0, PRIME - 1);

        coeffA = firstDistribution(rngEngine);
        coeffB = secondDistribution(rngEngine);
    }

    int operator()(int key, int mod){
        return abs(((coeffA * key + coeffB) % PRIME)) % mod;
    }
};

struct PerfectHashTable{
    UniversalHash primaryHash;
    UniversalHash** secondaryHashList;

    int keyCount;
    Record*** finalTable;

    Record*** stagingTable;
    int* stagingSizes;

    PerfectHashTable(int numKeys) : primaryHash(){
        keyCount = numKeys;
    }

    void build(int* keys, Record** entries){
        while(true){
            primaryHash.regenerate();
            stagingTable = new Record**[5 * keyCount];
            stagingSizes = new int[5 * keyCount];

            for(int i = 0; i < 5 * keyCount; i++){
                stagingTable[i] = new Record*[1];
                stagingSizes[i] = 0;
            }

            for(int i = 0; i < keyCount; i++){
                int bucket = primaryHash(keys[i], 5 * keyCount);
                stagingTable[bucket][stagingSizes[bucket]] = entries[i];
                stagingSizes[bucket]++;

                Record** expanded = new Record*[stagingSizes[bucket] + 1];
                for(int j = 0; j < stagingSizes[bucket]; j++){
                    expanded[j] = stagingTable[bucket][j];
                }

                delete[] stagingTable[bucket];
                stagingTable[bucket] = expanded;
            }

            int quadraticSum = 0;
            for(int i = 0; i < 5 * keyCount; i++){
                quadraticSum += stagingSizes[i] * stagingSizes[i];
            }

            if(quadraticSum <= 5 * keyCount){
                break;
            }
        }

        finalTable = new Record**[5 * keyCount];
        for(int i = 0; i < 5 * keyCount; i++){
            finalTable[i] = nullptr;
        }

        secondaryHashList = new UniversalHash*[5 * keyCount];
        for(int i = 0; i < 5 * keyCount; i++){
            secondaryHashList[i] = new UniversalHash();
        }

        for(int i = 0; i < 5 * keyCount; i++){
            while(true){
                int tableSize = stagingSizes[i] * stagingSizes[i] * 10;
                finalTable[i] = new Record*[tableSize];

                for(int j = 0; j < tableSize; j++){
                    finalTable[i][j] = nullptr;
                }

                UniversalHash* currHash = secondaryHashList[i];
                currHash->regenerate();

                bool clash = false;
                for(int j = 0; j < stagingSizes[i]; j++){
                    int pos =(*currHash)(stagingTable[i][j]->identifier, tableSize);
                    if(finalTable[i][pos]==nullptr){
                        finalTable[i][pos] = stagingTable[i][j];
                    } else{
                        clash = true;
                        break;
                    }
                }

                if(!clash) break;

                delete[] finalTable[i];
            }
        }
    }

    Record* lookup(int searchKey){
        int firstSlot = primaryHash(searchKey, 5 * keyCount);
        if(finalTable[firstSlot]==nullptr){
            return nullptr;
        }

        int secondSlot = secondaryHashList[firstSlot]->operator()(searchKey, 10 * stagingSizes[firstSlot] * stagingSizes[firstSlot]);
        return finalTable[firstSlot][secondSlot];
    }
};

struct CSVHandler{
    static int lineCounter(const string& fname){
        ifstream file(fname);
        string temp;
        int total = 0;
        while(getline(file, temp)) total++;
        return total;
    }

    static void parseCSV(const string& fname, int*& keys, Record**& records, int& total){
        total = lineCounter(fname) - 1;
        ifstream file(fname);
        string line;
        getline(file, line);

        keys = new int[total];
        records = new Record*[total];
        int idx = 0;

        while(getline(file, line)){
            stringstream ss(line);
            string idStr, keyStr, nm, mailAddr, sexStr;

            getline(ss, idStr, ',');
            getline(ss, keyStr, ',');
            getline(ss, nm, ',');
            getline(ss, mailAddr, ',');
            getline(ss, sexStr, ',');

            keys[idx] = stoll(keyStr);
            records[idx] = new Record;
            records[idx]->identifier = stoll(keyStr);
            records[idx]->name = nm;
            records[idx]->mail = mailAddr;
            records[idx]->sex = sexStr;
            idx++;
        }
    }
};

int32_t main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    int* keyArray;
    Record** valueArray;
    CSVHandler::parseCSV("data.csv", keyArray, valueArray, n);

    PerfectHashTable phTable(n);
    phTable.build(keyArray, valueArray);

    cout << "Hashing done, please enter : " << endl;
    int queryKey;
    cin >> queryKey;

    Record* result = phTable.lookup(queryKey);

    if(result==nullptr){
        cout << "Key not found\n";
    } else{
        cout << result->name << ", " << result->mail << ", " << result->sex << "\n";
    }
}

