template <typename T> string toString(T *val)
{
	char C[127];
	sprintf(C, "%s_@_%p", typeid(T).name(), val);
	return string(C);
}