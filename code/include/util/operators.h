#ifndef OPERATORS_GUARD
#define OPERATORS_GUARD
/*
// can't inherit operator=, since it is always overriden
// by derived class' copy-constructor.
// FFFFFFFFfffffuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuck
*/
template<typename T> class assignmentOperator final{
	public:
	virtual void operator=(const T& newval){set(newval);}
	virtual operator const T&(){return get();}
	virtual const T& get() = 0;
	virtual void set(const T&) = 0;
};


template<typename K, typename V> class subscriptOperator{
	public:
	virtual V& operator[](const K &key){return getIndexRef(key);}
	virtual V& getIndexRef(const K &key) = 0;
};

template<typename O, typename V> class deref final{
	public:
	O &owner;
	virtual void operator=(const V& newval){set(newval);}
	virtual operator const V&(){return get();}
	deref(O &newowner):owner(newowner){}
	virtual const V& get(){owner.get();}
	virtual void set(const V& val){owner.set(val);}
};

#define property(O, T, x)\
class property_##x{\
	public:\
	O &owner;\
	T val;\
	property_##x(O &newowner):owner(newowner){}\
	operator const T&(){return get();}\
	void operator=(const T& newval){set(newval);}\
	virtual const T& get();\
	virtual void set(const T&);\
} x = property_##x(*this);

template<typename K, typename O, typename V> class subscriptDeref final{
	public:
	O &owner;
	const K &key;

	subscriptDeref(const K &newkey, O &newowner):
		owner(newowner),key(newkey){}
	
	virtual void operator=(const V& newval){set(newval);}
	virtual operator const V&(){return get();}
	virtual const V& get(){return owner.getIndex(key);}
	virtual void set(const V& val){owner.setIndex(key,val);}
};

template<typename K, typename V> class subscriptAssignmentOperator
{
	public:
	virtual subscriptDeref<K,subscriptAssignmentOperator<K,V>,V> 
		operator[](const K &key){
			return subscriptDeref<K,subscriptAssignmentOperator<K,V>,V>(key,*this);
		}
	virtual const V& getIndex(const K &key) = 0;
	virtual void setIndex(const K &key, const V &value) = 0;
};

//--------------------- PROPERTY GET/SET EXPERIMENT ----------------------------



/*
class foo{
	public:
	//void seta(int& val);
	//const int& geta();
	//property<int> a(this,seta,geta);
	int b = 0;
	property(foo, int, a);
};
void foo::property_a::set(const int& newval){printf("setter fired (%d)\n",newval); owner.b++; val = newval;}
const int& foo::property_a::get(){printf("getter fired\n"); owner.b++; return val;}

void propertyExperiment(){
	printf("-------------- PROPERTY EXPERIMENT -----------\n");
	foo f;
	f.a = 5;
	int a = f.a;
	printf("int a == %d\n",a);
	printf("f.b = %d\n",f.b);
	printf("-------------- END PROPERTY EXPERIMENT -----------\n");
}
*/

//------------------------- END PROPERTY GET/SET EXPERIMENT


//--------- subscript operator experiment --------
/*
class barbuz:public subscriptOperator<string,int>{
	public:
	int I;
	virtual int& getIndexRef(const string &key) override{printf("look at my 'I'! %p\n",&I);return I;}
};

class barbook:public subscriptAssignmentOperator<string,int>{
	public:
	int I;
	virtual const int& getIndex(const string &key) override{printf("you wanna %s?\n",key.c_str()); return I;};
	virtual void setIndex(const string &key, const int &value) override{printf("da %s is now a %d\n",key.c_str(),value); I = value;}
};

void subscriptExperiment(){
	barbuz BZ;
	barbook BK;
	printf("-------------- subscript experiment ------------------\n");
	BZ["hello"] = 1;
	BK["BK"] = 2;
	int a = BZ["wat"];
	int b = BK["ok"];
	printf("a = %d, b = %d\n",a,b);
	printf("------------- end subscript experiment ---------------\n");
}
*/
//----- end subscript operator experiment -------


#endif

