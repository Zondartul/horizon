template<class T> class ptrSniper // I'm in way over my head.
{								  // an object that zero's all pointers to it's parent on destruction.
	public:
	vector<T**> pointers;
	T* upperThis;
	void get(T** ptr) // function takes adress of the pointer. Pointer's adress saved, pointer assigned to object.
	{
		for(int i=0;i<pointers.size();i++)
		{
			if(pointers[i]==ptr)
			{
				if(*ptr==upperThis)
				{printf("ptrSniper: ptr already defined\n");return;}
				else
				{printf("ptrSniper: refreshing ptr\n");*ptr = upperThis; return;}
			}
		}
		pointers.push_back(ptr);
		*ptr = upperThis;
		printf("ptrSniper: new ptr protected\n");
	}
	~ptrSniper()
	{
		printf("ptrSniper: sniping %d pointers\n", pointers.size());
		for(int i = 0; i<pointers.size(); i++)
		{
			if(*(pointers[i])==upperThis){*(pointers[i])=0;} //if no longer pointing here, do nothing.
		}
	}
};