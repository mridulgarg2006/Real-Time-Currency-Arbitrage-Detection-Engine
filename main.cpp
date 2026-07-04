#include<iostream>
#include<vector>
#include<string>
#include<unordered_map>
#include<cmath>
#include<limits>
#include<algorithm>

using namespace std;

struct exchangeedge{
    int source;
    int dest;
    double rate;
    double weight;
};

class arbitrageengine{
private:
    unordered_map<string,int> currencytoindex;
    unordered_map<int,string> indextocurrency;
    vector<exchangeedge> edges;
    int numvertices=0;

    int getindex(const string& currency){
        if(currencytoindex.find(currency)==currencytoindex.end()){
            currencytoindex[currency]=numvertices;
            indextocurrency[numvertices]=currency;
            numvertices++;
        }
        return currencytoindex[currency];
    }

public:
    void addexchangerate(const string& base,const string& quote,double rate){
        int u=getindex(base);
        int v=getindex(quote);
        
        double weight=-log(rate);
        edges.push_back({u,v,rate,weight});
    }

    void detectarbitrage(){
        if(numvertices==0) return;

        vector<double> dist(numvertices,0.0);
        vector<int> parent(numvertices,-1);
        int cyclestartnode=-1;

        for(int i=0;i<numvertices-1;++i){
            for(const auto& edge:edges){
                if(dist[edge.source]+edge.weight<dist[edge.dest]){
                    dist[edge.dest]=dist[edge.source]+edge.weight;
                    parent[edge.dest]=edge.source;
                }
            }
        }

        for(const auto& edge:edges){
            if(dist[edge.source]+edge.weight<dist[edge.dest]){
                cyclestartnode=edge.dest;
                parent[edge.dest]=edge.source;
                break;
            }
        }

        if(cyclestartnode==-1){
            cout<<"no arbitrage opportunities detected at this time.\n";
        }else{
            extractandprintcycle(parent,cyclestartnode);
        }
    }

private:
    void extractandprintcycle(const vector<int>& parent,int startnode){
        int current=startnode;
        for(int i=0;i<numvertices;++i){
            current=parent[current];
        }

        vector<int> cycle;
        int cycleorigin=current;
        do{
            cycle.push_back(current);
            current=parent[current];
        }while(current!=cycleorigin);
        
        cycle.push_back(cycleorigin);
        reverse(cycle.begin(),cycle.end());

        cout<<"\n[!] arbitrage opportunity detected [!]\n";
        cout<<"execution path: ";
        
        double profitmultiplier=1.0;
        for(size_t i=0;i<cycle.size()-1;++i){
            int u=cycle[i];
            int v=cycle[i+1];
            cout<<indextocurrency[u]<<" -> ";
            
            for(const auto& edge:edges){
                if(edge.source==u&&edge.dest==v){
                    profitmultiplier*=edge.rate;
                    break;
                }
            }
        }
        cout<<indextocurrency[cycle.back()]<<"\n";
        
        cout<<"profit multiplier: "<<profitmultiplier 
            <<" ("<<(profitmultiplier-1.0)*100.0<<"% profit per loop)\n";
    }
};

int main(){
    arbitrageengine engine;

    engine.addexchangerate("usd","eur",0.93);
    engine.addexchangerate("eur","gbp",0.86);
    engine.addexchangerate("gbp","usd",1.27);
    
    engine.addexchangerate("usd","gbp",0.79);
    engine.addexchangerate("eur","usd",1.07);
    engine.addexchangerate("gbp","eur",1.16);
    engine.addexchangerate("jpy","usd",0.0067);
    engine.addexchangerate("usd","jpy",149.50);

    engine.detectarbitrage();

    return 0;
}