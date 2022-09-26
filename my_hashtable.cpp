class Bucket{
private:
    vector<pair<int, int>> bucket;
public:
    Bucket(){
        //this->bucket = new vector<pair<int, int>>();
    }
    
    int get(int key) {
        for(auto& b:this->bucket) {
            if (b.first == key)
                return b.second;
        }
        return -1;
    }
    
    void update(int key, int value) {
        int found=false;
        for(auto& b:this->bucket) {
            if (b.first == key)
                found = true;
                b.second = value;
        }
        if (!found) this->bucket.push_back({key, value});
    }
    
    void remove(int key) {
        int tmp=0;
        for(auto& b:this->bucket) {
            if (b.first == key)
                this->bucket.erase(this->bucket.begin()+tmp);
                break;
            tmp++;
        }
    }
};


class MyHashMap {
private:
    int key_length;
    vector<Bucket> hash_table;
public:
    MyHashMap() {
        // 2069 not working for too many elements
        this->key_length = 206999;
        hash_table.resize(this->key_length);
    }
    
    void put(int key, int value) {
        int hash_key = key%this->key_length;
        this->hash_table[hash_key].update(key, value);
    }
    
    int get(int key) {
        int hash_key = key%this->key_length;
        return this->hash_table[hash_key].get(key);
    }
    
    void remove(int key) {
        int hash_key = key%this->key_length;
        this->hash_table[hash_key].remove(key);
    }
};

/**
 * Your MyHashMap object will be instantiated and called as such:
 * MyHashMap* obj = new MyHashMap();
 * obj->put(key,value);
 * int param_2 = obj->get(key);
 * obj->remove(key);
 */
