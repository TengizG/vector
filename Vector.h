#ifndef VECTOR123_H
#define VECTOR123_H
#include <memory>
#include <algorithm>
#include <iostream>


namespace one { class Vector; }

template <typename T,typename A=std::allocator<T> >
class Vector
{
	int sz;
	T* elem;
	int space;
	A alloc;
	
public:

	using iterator = T*;

	Vector() : sz(0), elem(nullptr), space(0) {};
	explicit Vector(int s) : sz(s), elem(new T[s]), space(s)
	{
		for (int i = 0; i < sz; ++i) elem[i] = 0;
	}

	Vector(int s,T val) : sz(s), elem(new T[s]), space(s)
	{
		for (int i = 0; i < sz; ++i) elem[i] = val;
	}

	Vector(Vector&&);
	Vector& operator=(Vector&&);

	Vector(const Vector&);
	Vector& operator=(const Vector&);

	~Vector() { delete[] elem; }

	double& operator[] (int n) { return elem[n]; }
	const double& operator[] (int n) const { return elem[n]; }

	int size() const { return sz; }
	int capasity() const { return space; }

	void resize(int newsize, T def = T());
	
	void push_back(const T& d);
	
	void reserve(int newaloc);

    void push_front(const T& d);

	iterator insert(iterator p, const T& val);
	iterator erase(iterator p);

	iterator begin() { return &elem[0]; }
	iterator end() { return &elem[sz]; }

	iterator back() { return &elem[sz - 1]; }
	
	friend std::istream& operator>>(std::istream& in, Vector<T, A>& val)
	{
		T t;
		in >> t;
		val.push_back(t);
		return in;
	}
	friend std::ostream& operator<<(std::ostream& out, Vector<T, A>& val)
	{
		for (int i = 0; i < val.size(); ++i)
			out << i << '.' << val.elem[i] << std::endl;
		return out;
	}


};




//---------------------------------------------------------------------
template<typename T, typename A> Vector<T, A>::Vector(const Vector& v) : sz(v.sz), elem(new T[sz]), space(v.sz)
{
	std::copy(v.elem, v.elem + sz, elem);
}


template<typename T, typename A> Vector<T, A>& Vector<T, A>::operator=(const Vector& v)
{
	if (this == &v) return *this;

	if (v.sz <= space)
	{
		std::copy(v.elem, v.elem + sz, elem);
		sz = v.sz;
		return *this;
	}

	double* p = new double[v.sz];
	std::copy(v.elem, v.elem + sz, p);
	delete[] elem;
	space = sz = v.sz;
	elem = p;
	return *this;
}

template<typename T, typename A> Vector<T, A>::Vector(Vector&& v) : sz(v.sz), elem(v.elem), space(v.space)
{
	v.sz = v.space = 0;
	v.elem = nullptr;
}

template<typename T, typename A> Vector<T, A>& Vector<T, A>::operator=(Vector&& v)
{
	delete[] elem;
	elem = v.elem;
	sz = space = v.sz;
	v.elem = nullptr;
	v.sz = v.space = 0;
	return *this;
}

template<typename T,typename A> void Vector<T,A>::reserve(int newaloc)
{
	if (newaloc <= space) return;

	
	T* p = alloc.allocate(newaloc);

	for (int i = 0; i < sz; ++i)
		alloc.construct(&p[i], elem[i]);
	for (int i = 0; i < sz; ++i)
		alloc.destroy(&elem[i]);
	alloc.deallocate(elem, space);
	elem = p;
	space = newaloc;
}


template<typename T, typename A> void Vector<T,A>::resize(int newsize,T def)
{
	reserve(newsize);
	for (int i = sz; i < newsize; ++i)
		alloc.construct(&elem[i], def);
	for (int i = newsize; i < sz; ++i)
		alloc.destroy(&elem[i]);
	sz = newsize;
}

template<typename T, typename A> void Vector<T,A>::push_back(const T& d)
{
	if (sz == 0)
		reserve(8);
	if (sz == space)
		reserve(2 * space);

	alloc.construct(&elem[sz], d);
	++sz;
}


template<typename T, typename A> typename Vector<T,A>::iterator Vector<T, A>::erase(iterator p)
{
	if (p == end()) return p;

	for (auto pos = p + 1; pos != end(); ++pos)
		*(pos - 1) = *pos;
	alloc.destroy(&*(end() - 1));
	--sz;
	return p;
}

template<class T, class A> typename Vector<T, A>::iterator Vector<T, A>::insert(iterator p, const T& val)
{
	int index = p - begin();
	if (size() == capasity())
		reserve(size() == 0 ? 8 : 2 * size());
	alloc.construct(elem + sz, *back());
	++sz;
	iterator pp = begin() + index;

	for (auto pos = end() - 1; pos != pp; --pos)
		*pos = *(pos - 1);
	*(begin() + index) = val;
	return pp;
}

template<class T, class A> void Vector<T, A>::push_front(const T& val)
{
	if (size() == capasity())
		reserve(size() == 0 ? 8 : 2 * size());
	if (sz == 0) 
		alloc.construct(elem, val);
		
	else {
		alloc.construct(elem + sz, *back());
			for (int i = sz - 1; i > 0; --i)
				elem[i] = elem[i - 1];
		elem[0] = val;
	}

	++sz;


}


/*template<class T, class A> std::istream& operator>>(std::istream& in , Vector<T,A>& val)
{
	T t;
	in >> t;
	val.push_back(t);
	return in;
}*/


/*template<class T, class A> std::ostream& operator<<(std::ostream& out, Vector<T, A>& val)
{
	for (int i = 0; i < val.size(); ++i)
		out <<i<<'.' <<*val.elem<<std::endl;
	return out;
}*/


#endif // !VECTOR123_H



