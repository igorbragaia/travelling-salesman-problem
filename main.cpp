#include<bits/stdc++.h>
using namespace std;
typedef pair<float,float> pp;
#define INF 2<<29


struct HeapElement{
    int weight, min_parent, index;
    HeapElement(int wweight, int mmin_parent, int iindex): weight(wweight), min_parent(mmin_parent), index(iindex) {};
    bool operator<(const HeapElement & x) const {
        if(weight < x.weight)
            return true;
        if(weight > x.weight)
            return false;
        if(min_parent < x.min_parent)
            return true;
        if(min_parent > x.min_parent)
            return false;
        return index < x.index;
    }
};


class Heap{
public:
    Heap(int n){
        size = 0;
        heap.resize(n + 1, HeapElement(-1,-1,-1));
        positions.resize(n + 1,-1);
    };

    HeapElement push(int weight, int min_parent, int index){
        HeapElement el(weight, min_parent, index);
        modify(++size, el);
    };

    HeapElement extractMin(){
        if(size < 1){
            throw invalid_argument("Heap underflow");
        } else if(size == 1){
            HeapElement top = heap[1];
            positions[top.index] = -1;
            size--;
            return top;
        } else if(size > 1){
            HeapElement top = heap[1];
            positions[top.index] = -1;
            modify(1, heap[size--]);
            return top;
        }
    };

    void modify(int k, HeapElement el){
        if(k < 1 or k > size){
            throw invalid_argument("Index error");
        } else {
            heap[k] = el;
            positions[heap[k].index] = k;

            while(k > 1 and heap[k] < heap[k/2]){
                HeapElement aux = heap[k];
                heap[k] = heap[k/2];
                heap[k/2] = aux;

                positions[heap[k].index] = k;
                positions[heap[k/2].index] = k/2;

                k = k/2;
            }
            sift(k);
        }
    }

    bool hasVertex(int k){
        return positions[k] != -1;
    }

    int getVertex(int k){
        return positions[k];
    }

    HeapElement heapVertex(int k){
        return heap[positions[k]];
    }

    bool empty(){
        return !(size > 0);
    }
    int size;
    vector<HeapElement>heap;
    vector<int>positions;
private:

    void sift(int k){
        int esq = 2*k, dir = 2*k+1, menor = k;
        if(esq <= size and heap[esq] < heap[menor])
            menor = esq;
        if(dir <= size and heap[dir] < heap[menor])
            menor = dir;
        if(menor != k){
            HeapElement aux = heap[k];
            heap[k] = heap[menor];
            heap[menor] = aux;

            positions[heap[k].index] = k;
            positions[heap[menor].index] = menor;

            sift(menor);
        }
    };
};


class Solution{
public:
    Solution(vector<pp>&vs, unsigned int&v){
        total_vertices = v;
        vertices = vs;
        int start = 1;
        prim(start);
        create_cycle(start);
    }

    void print_cycle(){
        for(int i = 0; i < (int)cycle.size(); i++)
            printf("%d ", cycle[i]);
        printf(("\n"));
    }

    int getCost(){
        return cost;
    }
private:
    int total_vertices;
    vector<pp>vertices;

    vector<vector<int>>mst;

    vector<bool>visited;
    vector<int>cycle;
    int cost;

    void prim(int start){
        mst.resize(total_vertices+1);
        Heap heap((int)total_vertices+1);
        for(int i = 1; i<= total_vertices; i++){
            // printf("%d %d %d\n", dij(vertices[start], vertices[i]), start, i);
            heap.push(dij(vertices[start], vertices[i]), start, i);
        }


        while(!heap.empty()){
            HeapElement top = heap.extractMin();
            printf("top %d\n", top.index);

            if(top.index != top.min_parent){
                mst[top.index].push_back(top.min_parent);
                mst[top.min_parent].push_back(top.index);
            }

            for(int i = 1; i <= total_vertices; i++){
                if(i != top.index and heap.hasVertex(i)){
                    if((dij(vertices[i], vertices[top.index]) < heap.heapVertex(i).weight)
                       or (dij(vertices[i], vertices[top.index]) == heap.heapVertex(i).weight and top.index < heap.heapVertex(i).min_parent)){
                        printf("## %d %d\n", heap.heapVertex(i).index, i);
                        HeapElement he(dij(vertices[i], vertices[top.index]), top.index, i);
                        heap.modify(heap.getVertex(i), he);
                    }
                }
            }
        }
    }

    void create_cycle(int start){
        visited.resize(total_vertices+1);
        for(int i = 1; i <= (int)total_vertices; i++)
            visited[i] = false;

        visited[start] = true;
        create_cycle_path(start);
        cycle.push_back(start);

        cost = 0;
        for(int i = 1; i < (int)cycle.size(); i++){
            cost += dij(vertices[i], vertices[i-1]);
        }
    }

    void create_cycle_path(int vertex){
        cycle.push_back(vertex);
        sort(mst[vertex].begin(), mst[vertex].end());
        for(int neighbor:mst[vertex])
            if(!visited[neighbor]){
                visited[neighbor] = true;
                create_cycle_path(neighbor);
            }
    }

    static int dij(pp i, pp j){
        float xd = i.first - j.first, yd = i.second - j.second;
        float x = sqrt(xd*xd + yd*yd);
        return (int)(x + 0.5);
    }
};


int main(){
    int n=1;
//    cin >> n;

    ofstream writingfile;
    writingfile.open("saida.txt");

    /* vertices variables */
    unsigned int total_vertices;
    int vertice;
    float x, y;
    vector<pp> vertices;
    /* vertices variables */

    for(int i = 1; i <= n; i++){
        string filename;
        filename = "/home/igor/Documentos/travelling-salesman-problem/ent01.txt";//"ent"  + (i<10)?"0":"" + to_string(i) + ".txt";

        ifstream readingfile(filename);

        readingfile >> total_vertices;
        vertices.clear();
        vertices.resize(total_vertices + 1);

        for(unsigned int j = 1; j <= total_vertices; j++){
            readingfile >> vertice >> x >> y;
            vertices[vertice] = make_pair(x,y);
        }

        readingfile.close();

        Solution * solve = new Solution(vertices,total_vertices);
        printf("%d\n",solve->getCost());
        solve->print_cycle();
        delete solve;
    }
    writingfile.close();

    return 0;
}
