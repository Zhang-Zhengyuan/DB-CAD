/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined( _ISCMPickSubscriber_)
#define _ISCMPickSubscriber_

#include "mmgr.hxx"

class ENTITY;
class component_entity_handle;

class ISCMPickSubscriber: public ACIS_OBJECT
{
public:
	// ********************************************************************
	// Description	: The pick manager will call this method when it is notified
	//				  that a display object has been picked.
	//
	//
	virtual void	// Returns nothing.
		NotifyAboutPick(
		ENTITY*
		) = 0;

	// ********************************************************************
	// Description	: The pick manager will call this method when it is notified
	//				  that a display object has been unpicked.
	//
	//
	virtual void	// Returns nothing.
		NotifyAboutUnPick(
		ENTITY*
		) = 0;

	// ********************************************************************
	// Description	: The pick manager will call this method when it is notified
	//				  that a display object of an assembly has been picked.
	//
	//
	virtual void	// Returns nothing.
		NotifyAboutPickAsm(
		component_entity_handle*
		) {};

	// ********************************************************************
	// Description	: The pick manager will call this method when it is notified
	//				  that a display object of an assembly has been unpicked.
	//
	//
	virtual void	// Returns nothing.
		NotifyAboutUnPickAsm(
		component_entity_handle*
		) {};

	// ********************************************************************
	// Description	: Remove/Unpick all display objects from the list
	//
	//
	virtual void	// Returns nothing.
		ClearENTITYs() = 0;

	// ********************************************************************
	// Description	: The pick manager will call this method when it is notified
	//				  that one or more display objects are about to be picked
	//				  or unpicked.
	//
	//
	// Notes		: 
	//					
	virtual void	// Returns nothing.
		BeginPick() = 0;

	// ********************************************************************
	// Description	: The pick manager will call this method when it is done
	//				  notifying everyone about display objects that were picked
	//				  or unpicked.
	//
	//
	// Notes		: 
	//					
	virtual void	// Returns nothing.
		EndPick() = 0;

	// ********************************************************************
	// Description	: Picking is complete. This is a good point in time to 
	//				  refresh the display.
	//
	//
	// Notes		: 
	//					
	virtual void	// Returns nothing.
		Refresh() = 0;

	virtual long AddRef() = 0;
	virtual long Release() = 0;
};

class ISCMPickSubscriberPTR_List: public ACIS_OBJECT
{
public:
	class list_node: public ACIS_OBJECT
	{
	public:
		list_node(ISCMPickSubscriber* value):
		m_Value(value), m_Next(0), m_Prev(0){}

		~list_node()
		{
//			if (m_Next)
//				delete m_Next;
		}

		list_node *&next() { return m_Next;}

		logical operator < (const list_node &node) const {return m_Value < node.m_Value;}
		logical operator > (const list_node &node) const {return m_Value > node.m_Value;}
		logical operator == (const list_node &node) const {return m_Value == node.m_Value;}
		
		ISCMPickSubscriber*	m_Value;
		list_node *m_Next;
		list_node *m_Prev;

		class iterator: public ACIS_OBJECT
		{
			public:
			list_node *m_Node;
			iterator ():m_Node(0){}
			iterator (list_node *node):m_Node(node){}

			iterator &operator ++ () 
			{
				if (m_Node)
					m_Node=m_Node->m_Next;
				return *this;
			}
			iterator &operator -- () 
			{
				if (m_Node)
					m_Node=m_Node->m_Prev;
				return *this;
			}
			void operator =(list_node *node) {m_Node=node;}
			ISCMPickSubscriber* &operator *() { return m_Node->m_Value;}
//			ISCMPickSubscriber* *operator ->() { return &(m_Node->m_Value);}

			logical operator !=(const iterator &iter) const {return m_Node!=iter.m_Node;}
			logical operator ==(const iterator &iter) const {return m_Node==iter.m_Node;}
			logical operator ==(const list_node *node) const {return m_Node==node;}
		};
		class reverse_iterator: public ACIS_OBJECT
		{
			public:
			list_node *m_Node;
			reverse_iterator ():m_Node(0){}
			reverse_iterator (list_node *node):m_Node(node){}

			reverse_iterator &operator ++ () 
			{
				if (m_Node)
					m_Node=m_Node->m_Prev;
				return *this;
			}
			reverse_iterator &operator -- () 
			{
				if (m_Node)
					m_Node=m_Node->m_Next;
				return *this;
			}
			void operator =(list_node *node) {m_Node=node;}
			ISCMPickSubscriber* &operator *() { return m_Node->m_Value;}
//			ISCMPickSubscriber* *operator ->() { return &(m_Node->m_Value);}

			logical operator !=(const reverse_iterator &iter) const {return m_Node!=iter.m_Node;}
			logical operator ==(const reverse_iterator &iter) const {return m_Node==iter.m_Node;}
			logical operator ==(const list_node *node) const {return m_Node==node;}
		};

		class const_iterator: public ACIS_OBJECT
		{
			public:
			list_node *m_Node;
			const_iterator ():m_Node(0){}
			const_iterator (list_node *node):m_Node(node){}
			const_iterator (const iterator &iter):m_Node(iter.m_Node){}

			const_iterator &operator ++ () 
			{
				if (m_Node)
					m_Node=m_Node->m_Next;
				return *this;
			}
			const_iterator &operator -- () 
			{
				if (m_Node)
					m_Node=m_Node->m_Prev;
				return *this;
			}
			void operator =(list_node *node) {m_Node=node;}
			ISCMPickSubscriber* &operator *() { return m_Node->m_Value;}
//			ISCMPickSubscriber* *operator ->() { return &(m_Node->m_Value);}

			logical operator !=(const const_iterator &iter) const {return m_Node!=iter.m_Node;}
			logical operator ==(const const_iterator &iter) const {return m_Node==iter.m_Node;}
			logical operator ==(const list_node *node) const {return m_Node==node;}
		};
	};

	list_node *m_Begin,*m_End;
	unsigned int m_Size;
public:

	typedef list_node::iterator iterator;
	typedef list_node::const_iterator const_iterator;
	typedef list_node::reverse_iterator reverse_iterator;

	ISCMPickSubscriberPTR_List():m_Begin(0),m_End(0),m_Size(0) {};
	~ISCMPickSubscriberPTR_List()
	{
		clear();
	}

	ISCMPickSubscriber* &front() {return m_Begin->m_Value;}
	ISCMPickSubscriber* &back() {return m_End->m_Value;}
	ISCMPickSubscriber* &top() {return back();}

	void push_back(ISCMPickSubscriber* &value)
	{
		list_node *new_node=ACIS_NEW list_node(value);
		if (!m_Begin)
			m_Begin=new_node;
		if (m_End)
		{
			m_End->next()=new_node;
			new_node->m_Prev=m_End;
		}
		m_End=new_node;
		m_Size++;
	}
	void push(ISCMPickSubscriber* &value)
	{
		push_back(value);
	}

	void push_front(ISCMPickSubscriber* &value)
	{
		list_node *new_node=ACIS_NEW list_node(value);
		if (!m_Begin)
			m_Begin=new_node;
		else
		{
			new_node->next()=m_Begin;
			m_Begin->m_Prev=new_node;
			m_Begin=new_node;
		}
		m_Size++;
	}

	void pop_back()
	{
		erase(iterator(m_End));
	}
	void pop()
	{
		pop_back();
	}

	void pop_front()
	{
		erase(iterator(m_Begin));
	}

	iterator begin() {return iterator(m_Begin);}
	iterator end() {return iterator(0);}

	const_iterator begin() const {return iterator(m_Begin);}
	const_iterator end() const {return const_iterator(0);}

	reverse_iterator rbegin() {return reverse_iterator(m_End);}
	reverse_iterator rend() {return reverse_iterator(0);}

	void clear () 
	{
		while (m_Begin)
		{
			list_node *node_to_delete=m_Begin;
			m_Begin=m_Begin->next();
			node_to_delete->m_Next=0;
			node_to_delete->m_Prev=0;
			ACIS_DELETE node_to_delete;
		}

		m_Begin=0;
		m_End=0;
		m_Size=0;
	}
	
	unsigned int size() const {return m_Size;}
	logical empty() const {return m_Begin==0;}
	iterator find(const ISCMPickSubscriber* const &value)
	{
		if (!m_Begin) return 0;
		iterator iter=m_Begin;
		while (iter!=end() && (iter.m_Node->m_Value != value))
			++iter;

		return iter;
	}

	void erase(const iterator &i)
	{
		iterator del_iter=i;
		if (del_iter==end()) 
			return;

		if (del_iter.m_Node == m_Begin)
		{
			m_Begin=m_Begin->next();
			if (!m_Begin)
				m_End=0;
		}
		else
		{
			iterator iter=m_Begin;
			while (del_iter.m_Node != iter.m_Node->m_Next)
				++iter;
			if (del_iter.m_Node==m_End)
				m_End=iter.m_Node;
			iter.m_Node->m_Next=del_iter.m_Node->m_Next;
			if (del_iter.m_Node->m_Next)
				del_iter.m_Node->m_Next->m_Prev=iter.m_Node;
		}
		del_iter.m_Node->m_Next=0;
		ACIS_DELETE del_iter.m_Node;del_iter.m_Node=0;
		m_Size--;
	}
	void remove(const ISCMPickSubscriber* const &value)
	{
		erase(find(value));
	}
	void sort()
	{
		list_node *temp_list_begin=m_Begin;
		m_Begin=m_End=0;

		// Insert the dudes in the right order.
		while (temp_list_begin)
		{
			// Get a node to insert into the sorted list.
			list_node *node_to_insert=temp_list_begin;
			// Iterate to the next node.
			temp_list_begin=temp_list_begin->m_Next;
			// Make the node to insert "stand alone"
			node_to_insert->m_Next=0;
			node_to_insert->m_Prev=0;

			if (!m_Begin)
			{
				m_Begin=m_End=node_to_insert;
			}
			else if (*node_to_insert<*m_Begin || *node_to_insert==*m_Begin)
			{
				node_to_insert->m_Next=m_Begin;
				m_Begin->m_Prev=node_to_insert;
				m_Begin=node_to_insert;
			}
			else
			{
				iterator iter=m_Begin;
				while (iter!=end() && *node_to_insert > *iter)
//				while (iter!=end() && *iter < *node_to_insert )
				{
					++iter;
				}
				// Node needs to be inserted at end.
				if (iter==end())
				{
					m_End->m_Next=node_to_insert;
					node_to_insert->m_Prev=m_End;
					m_End=node_to_insert;
				}
				else
				{
					node_to_insert->m_Next=iter.m_Node;
					node_to_insert->m_Prev=iter.m_Node->m_Prev;
					iter.m_Node->m_Prev->m_Next=node_to_insert;
					iter.m_Node->m_Prev=node_to_insert;
				}
			}
		}
	}
	void unique()
	{
		iterator next_unique=m_Begin;
		iterator next_node=next_unique;
		while (next_unique!=end())
		{
			do
			{
				++next_node;
				if (next_node!=end() && *next_unique==*next_node)
				{
					next_unique.m_Node->m_Next=next_node.m_Node->m_Next;
					if (next_unique.m_Node->m_Next)
						next_unique.m_Node->m_Next->m_Prev=next_unique.m_Node;
					next_node.m_Node->m_Next=0;
					if (m_End==next_node.m_Node)
						m_End=next_unique.m_Node;
					delete next_node.m_Node;next_node.m_Node=0;
					next_node=next_unique;
					m_Size--;
				}
				else
				{
					break;
				}
			} while (next_node!=end());
			++next_unique;
			next_node=next_unique;
		}
	}
};

#endif
