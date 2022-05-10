#ifndef OPERATORS_GUARD
#define OPERATORS_GUARD

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















#endif

