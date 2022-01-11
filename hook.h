class hookKind{
	public:
	map<string,vector<pair<string, funcptr>>> hooks;
	void add(string type, string name, funcptr func){
		hooks[type].push_back({name, func});
	}
	void run(string type){
		vector<pair<string, funcptr>> V = hooks[type];
		for(int I = 0; I<V.size(); I++){
			V[I].second(NULL);
		}
	}
	void remove(string type, string name){
		for(int I = 0; I<hooks[type].size(); I++){
			if(hooks[type][I].first == name){
				//printf("found it at %d\n",I);
				hooks[type].erase(hooks[type].begin()+I);
				I--;
			}
		}
	}
}hook;