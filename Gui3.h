//  333333
// 33    33
//       33
//     333
//       33
// 33    33
//  333333

// okay lets make this count

class GUI3base
{
	private:
	rect posLocal;
	rect posGlobal;
	GUI3base *parent;
	list<*GUI3base> children;
	public:
	GUI3base()
	{
		printf("GUI3base created.")
	}
	void setParent(GUI3base *parent)
	{
		this.parent = parent;
		parent.children.insert(this);
	}
}