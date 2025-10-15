/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef _SCMPickManager_hxx_
#define _SCMPickManager_hxx_
/**
 * @file scmpickmgr.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup SCMAPI
 * \Declared at <scmpickmgr.hxx>
 * @{
 */
#include "logical.h"
#include "iscmpicksub.hxx"
#include "ent_cb.hxx"
#include "scm_eval.hxx"
#include "lists.hxx"
class component_entity_handle;
class component_entity_handle_list;
/**
 * The scm_pick_manager object.
 * <br><br>
 * A "scm_pick_manager" is a class which implements the ability to manage a 
 * list of "picked" ENTITY's.  It is part of our Scheme application, so it merely serves
 * as an example of how to implement such a class.
 * <br><br>
 * This class 
 * <br><br>
 * As this class is viewed as a way to acheive the described goal, how it is implemented
 * is up to the application. Hence, we leave the details of this class undocumented.
 * <br><br>
 * This object is used by the PART_CONTEXT class.
 * @see PART_CONTEXT
 */
class scm_pick_manager : public entity_callback
{
public:
	/**
	 * @nodoc
	 */
	scm_pick_manager();
	/**
	 * @nodoc
	 */
	virtual ~scm_pick_manager();

private:
	ENTITY_LIST	*m_PickedENTITYs;
	component_entity_handle_list *m_PickedComponentENTITYs;
	ISCMPickSubscriberPTR_List m_SubscriberList;
	logical m_DuringRoll;

public:

	/**
	 * @nodoc
	 */
	void AddENTITY(ENTITY*, logical invert = FALSE);
	/**
	 * @nodoc
	 */
	void AddENTITYs(ENTITY_LIST&, logical invert = FALSE);
	/**
	 * @nodoc
	 */
	void AddComponentENTITY(component_entity_handle*, logical invert = FALSE);
	/**
	 * @nodoc
	 */
	void AddComponentENTITYs(component_entity_handle_list&, logical invert = FALSE);

	/**
	 * @nodoc
	 */
	void RemoveENTITY(ENTITY*);
	/**
	 * @nodoc
	 */
	void RemoveENTITYs(ENTITY_LIST*);
	/**
	 * @nodoc
	 */
	void RemoveComponentENTITY(component_entity_handle*);
	/**
	 * @nodoc
	 */
	void RemoveComponentENTITYs(component_entity_handle_list*);

	/**
	 * @nodoc
	 */
	logical ContainsENTITY(ENTITY*);
	/**
	 * @nodoc
	 */
	logical ContainsComponentENTITY(component_entity_handle*);

	/**
	 * @nodoc
	 */
	void ClearENTITYs();
	/**
	 * @nodoc
	 */
	void ClearComponentENTITYs();
	
	/**
	 * @nodoc
	 */
	unsigned get_ENTITYCount() const { return m_PickedENTITYs->iteration_count(); }
	
	/**
	 * @nodoc
	 */
	logical HasENTITYs() const { return m_PickedENTITYs->iteration_count() > 0; }
	
	/**
	 * @nodoc
	 */
	ENTITY_LIST& get_ENTITY_LIST()
	{
		return *m_PickedENTITYs;
	}

	// Subscriber interfaces //////////////////////////////////////////////////

	/**
	 * @nodoc
	 */
	void Subscribe(ISCMPickSubscriber*);

	/**
	 * @nodoc
	 */
	void UnSubscribe(ISCMPickSubscriber*);
	
	/**
	 * @nodoc
	 */
	logical ContainsSubscriber(ISCMPickSubscriber*) const;

	/**
	 * @nodoc
	 */
	void ClearSubscribers()
	{
		m_SubscriberList.clear();
	}
	
	/**
	 * @nodoc
	 */
	unsigned get_SubscriberCount() const
	{
		return m_SubscriberList.size();
	}
	
	/**
	 * @nodoc
	 */
	logical HasSubscribers() const
	{
		return !m_SubscriberList.empty();
	}

	/**
	 * @nodoc
	 */
	void BeginPick();
	/**
	 * @nodoc
	 */
	void EndPick();
	/**
	 * @nodoc
	 */
	void Refresh();

	// entity_callback

	/**
	 * @nodoc
	 */
	void execute(entity_event_type, ENTITY* );
};


/**
 * @nodoc
 */
class scm_script_ISCMPickSubscriber : public ISCMPickSubscriber
{
	long m_Refs;
	char *m_Script;
	char *m_Command;
public:
	scm_script_ISCMPickSubscriber():m_Refs(0),m_Script(0),m_Command(0){}
	~scm_script_ISCMPickSubscriber()
	{
		if (m_Script)
		{
			ACIS_DELETE [] STD_CAST m_Script;
			m_Script=0;
		}
		if (m_Command)
		{
			ACIS_DELETE [] STD_CAST m_Command;
			m_Command=0;
		}
	}

	virtual void NotifyAboutPick(ENTITY*)	{run_cmd("\"NotifyAboutPick\"");}
	virtual void NotifyAboutUnPick(ENTITY*)	{run_cmd("\"NotifyAboutUnPick\"");}
	virtual void NotifyAboutPickAsm(component_entity_handle*)	{run_cmd("\"NotifyAboutPickAsm\"");}
	virtual void NotifyAboutUnPickAsm(component_entity_handle*)	{run_cmd("\"NotifyAboutUnPickAsm\"");}
	virtual void ClearENTITYs()				{run_cmd("\"ClearENTITYs\"");}
	virtual void BeginPick()				{run_cmd("\"BeginPick\"");}
	virtual void EndPick()					{run_cmd("\"EndPick\"");}
	virtual void Refresh()					{run_cmd("\"Refresh\"");}

	virtual void set_Script(const char *s)
	{
		size_t length = strlen(s);
		if (m_Script)
		{
			ACIS_DELETE [] STD_CAST m_Script;
			m_Script=0;
		}
		if (m_Command)
		{
			ACIS_DELETE [] STD_CAST m_Command;
			m_Command=0;
		}

		m_Script=ACIS_NEW char[length+1];
		strcpy(m_Script,s);

		m_Command=ACIS_NEW char[length+50];
	}
	virtual const char *get_Script(){return m_Script;}

	virtual void run_cmd(const char *c)
	{
		if (m_Script)
		{
			sprintf(m_Command,"(%s %s)",m_Script,c);
			ScmObject scheme_result=make_unspecified();
			SchemeEvaluate(m_Command, scheme_result );
		}
	}

	long AddRef() { return m_Refs++; }
	long Release() { long r = m_Refs; if(--m_Refs <= 0) ACIS_DELETE this; return r;}
};
/** @} */
#endif
