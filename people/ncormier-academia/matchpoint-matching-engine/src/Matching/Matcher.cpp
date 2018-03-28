/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :   
 * Desc    :    
 * Author  :    mat
 *****************************************************************************/


#include "Matcher.h"
#include "Attributes.h"
#include "MString.h"
#include "MatchTask.h"
#include "SQLGenerator.h"
#include "Attributes.h"
#include "UserRequest.h"
#include "MatchRules.h"
#include "Interfaces.h"
#include "Instances.h"
#include "DataAnswer.h"
#include "MatchAnswer.h"

DNSPACE

/**************************
 * struct node
 **************************/
struct node
{
  struct node  *p_next;

  int          i_type;
  void         *p_value;
  int          ( *pf_compare )( void *p_v1, void *p_v2 );

  int          i_children;
  struct node  *p_children;

  int          i_count;
};

enum
  {
    TYPE_INT,
    TYPE_STRING
  };

/*******************************************************************************
 * compare_string
 ******************************************************************************/
static int compare_string( void *p_v1, void *p_v2 )
{
  return !strcmp( (char*)p_v1, (char*)p_v2 );
}

/*******************************************************************************
 * compare_int
 ******************************************************************************/
static int compare_int( void *p_v1, void *p_v2 )
{
  return (int)p_v1 == (int)p_v2;
}

/*******************************************************************************
 * create_node
 ******************************************************************************/
static struct node *create_node( int i_type, void *p_value, int ( *pf_compare )( void *p_v1, void *p_v2 ) )
{
  struct node  *p_node = (struct node*)malloc( sizeof( *p_node ) );
  if( p_node == NULL )
    {
      fprintf( stderr, "not enough memory\n" );
      return NULL;
    }

  p_node->p_next = NULL;

  p_node->i_type = i_type;
  switch( i_type )
    {
    case TYPE_INT:
      p_node->p_value = p_value;
      break;

    case TYPE_STRING:
      p_node->p_value = strdup( (char*)p_value );
      break;

    default:
      fprintf( stderr, "unknown type\n" );
      free( p_node );
      return NULL;
    }
  p_node->pf_compare = pf_compare;
  p_node->i_children = 0;
  p_node->p_children = NULL;
  p_node->i_count = 0;

  return p_node;
}

/*******************************************************************************
 * update_leaf
 ******************************************************************************/
static int update_leaf( struct node *p_root, const char *psz_name, int i_valuetype, void *p_value, int i_count )
{
  /* search the parent node */
  for( struct node  *p_node = p_root->p_children; p_node != NULL; p_node = p_node->p_next )
    {
      if( strcmp( (char*)p_node->p_value, psz_name ) == 0 )  /* we known it is a string here */
	{
	  /* we have the parent, now look through children */
	  for( struct node  *p_children = p_node->p_children; p_children != NULL; p_children = p_children->p_next )
	    {
	      if( p_children->pf_compare( p_children->p_value, p_value ) == 1 )
		{
		  /* we found the good child, update */
		  p_children->i_count += i_count;
		  return 1;
		}
	    }

	  /* no child, create one and update */
	  struct node  *p_children = NULL;

	  switch( i_valuetype )
	    {
	    case TYPE_INT:
	      p_children = create_node( i_valuetype, p_value, compare_int );
	      break;

	    case TYPE_STRING:
	      p_children = create_node( i_valuetype, p_value, compare_string );
	      break;

	    default:
	      fprintf( stderr, "unknown type\n" );
	      return 0;
	    }

	  if( p_children == NULL )
	    return 0;

	  if( i_valuetype == TYPE_INT )
	    {
	      /* try to sort-insert int values for better performance */
	      struct node  *p_prev = NULL;
	      struct node  *p_tmp = NULL;
	      for( p_tmp = p_node->p_children; p_tmp != NULL; p_tmp = p_tmp->p_next )
		{
		  if( (int)p_tmp->p_value > (int)p_value )
		    break;

		  p_prev = p_tmp;
		}

	      if( p_prev == NULL )
		{
		  p_children->p_next = p_node->p_children;
		  p_node->p_children = p_children;
		  p_node->i_children++;
		}
	      else
		{
		  p_children->p_next = p_prev->p_next;
		  p_prev->p_next = p_children;
		}
	    }
	  else
	    {
	      p_children->p_next = p_node->p_children;
	      p_node->p_children = p_children;
	      p_node->i_children++;
	    }

	  /* update */
	  p_children->i_count += i_count;

	  return 1;
	}
    }

  return 0;
}

/*******************************************************************************
 * lookup
 ******************************************************************************/
static int lookup( struct node *p_root, const char *psz_name, void *p_value )
{
  for( struct node  *p_node = p_root->p_children; p_node != NULL; p_node = p_node->p_next )
    {
      /* browse each parent node */
      if( strcmp( (char*)p_node->p_value, psz_name ) == 0 )
	{
	  if( p_value == NULL )
	    return 1;

	  /* one we find the parent, look for a child according to types */
	  for( struct node  *p_children = p_node->p_children; p_children != NULL; p_children = p_children->p_next )
	    {
	      if( p_children->pf_compare( p_children->p_value, p_value ) == 1 )
		{
		  return p_children->i_count;
		}
	    }

	  return 0;
	}
    }

  return 0;
}

/*******************************************************************************
 * dump_node
 ******************************************************************************/
static void dump_node( struct node *p_root, const char *psz_name )
{
  printf( "[%s]\n", psz_name );
  for( struct node  *p_node = p_root->p_children; p_node != NULL; p_node = p_node->p_next )
    {
      if( strcmp( (char*)p_node->p_value, psz_name ) == 0 )
	{
	  for( struct node  *p_children = p_node->p_children; p_children != NULL; p_children = p_children->p_next )
	    {
	      switch( p_children->i_type )
		{
		case TYPE_INT:
		  printf( "\t--> %i (%i)\n", (int)p_children->p_value, p_children->i_count );
		  break;

		case TYPE_STRING:
		  printf( "\t--> %s (%i)\n", (char*)p_children->p_value, p_children->i_count );
		  break;
		}
	    }

	  break;
	}
    }
}

/*******************************************************************************
 * Match
 ******************************************************************************/
void Matcher::Match( MatchTask& tsk, const DataAnswer &dataAnswer )
{
  static struct node  *p_root = NULL;

  if( p_root == NULL )
    p_root = create_node( TYPE_STRING, (void*)"root", NULL );

  int  *p_totals = NULL;
  int  i_totals = 0;
#ifndef _WIN32
  int  i_chosen[dataAnswer.ObjectsCount()];
#else
  int  *i_chosen = new int[dataAnswer.ObjectsCount()];
#endif

  /* the maximum number of results */
  int  i_max = (int)( dataAnswer.ObjectsCount() * ( (float)tsk.m_mutableUserRequest->MinMatchPercentage() / 100.0f ) );
  int  i_goodcol = -1;

  memset( i_chosen, 0, sizeof( i_chosen ) );

  for( unsigned  j = 0; j < dataAnswer.ObjectsCount(); j++ )
    {
      const ObjectInstance&  obj = dataAnswer.ObjectAtIndex(j);

      /* if no array of parents' name, create it */
      if( p_totals == NULL )
	{
	  p_totals = (int*)calloc( 1, obj.AttributeCount() * sizeof( *p_totals ) );
	  i_totals = obj.AttributeCount();
	}

#ifndef _WIN32
      int  i_cols[obj.AttributeCount()];
#else
      int  *i_cols = new int[obj.AttributeCount()];
#endif
      memset( i_cols, 0, sizeof( i_totals ) );

      /* for each attribute, retrieve the number of matching entries already seen */
      for( unsigned int  i = 0, n = obj.AttributeCount(); i < n; i++ )
	{
	  const ObjectInstanceAttribute  *p_instanceAttr = &(obj.AttributeAtIndex( i ));
	  FASSERT( IS_INVALID_OBJECT_INSTANCE_ATTRIBUTE( *p_instanceAttr ) == false, "Invalid object instance attribute" );

	  /* check if this pattern already exists */
	  char buf[128];
	  sprintf( buf, "%s.%s", p_instanceAttr->Attribute().ParentInterface().TableName().CharArray(), p_instanceAttr->Attribute().ColumnName().CharArray() );

	  if( lookup( p_root, buf, NULL ) == 0 )
	    {
	      /* create it */
	      struct node  *p_node = create_node( TYPE_STRING, (void*)buf, NULL );

	      p_node->p_next = p_root->p_children;
	      p_root->p_children = p_node;
	      p_root->i_children++;
	    }

	  /* store the number of entries in an array */

	  int  i_count = 0;

	  const IAttributeValue &value = p_instanceAttr->Value();

	  switch (value.TypeOf())
	    {
	    case (AttributeValueType::INTEGER):
	      {
		const AttributeValue<int> &val = dynamic_cast<const AttributeValue<int> &>(value);
		i_count = lookup( p_root, buf, (void*)val.Value() );
	      }
	      break;

	    case (AttributeValueType::STRING):
	      {
		const AttributeValue<MString> &val = dynamic_cast<const AttributeValue<MString> &>(value);
		i_count = lookup( p_root, buf, (void*)val.Value().CharArray() );
	      }
	      break;
	    }

	  i_cols[i] = i_count;
	}

      /* for all columns of this row, look for the min and store it */
      int i_min = -1;
      for( int k = 0; k < obj.AttributeCount(); k++ )
	{
	  if( ( i_cols[k] < i_min ) || ( i_min == -1 ) )
	    {
	      i_min = i_cols[k];
	      i_goodcol = k;
	    }
	}

      /* check if we can add this row according to the global stats */
      if( p_totals[i_goodcol] < i_cols[i_goodcol] )
	{
	  const ObjectInstanceAttribute  *p_instanceAttr = &(obj.AttributeAtIndex( i_goodcol ));
	  FASSERT( IS_INVALID_OBJECT_INSTANCE_ATTRIBUTE( *p_instanceAttr ) == false, "Invalid object instance attribute" );

	  const IAttributeValue &value = p_instanceAttr->Value();

	  char buf[128];
	  sprintf( buf, "%s.%s", p_instanceAttr->Attribute().ParentInterface().TableName().CharArray(), p_instanceAttr->Attribute().ColumnName().CharArray() );

	  switch (value.TypeOf())
	    {
	    case (AttributeValueType::INTEGER):
	      {
		const AttributeValue<int> &val = dynamic_cast<const AttributeValue<int> &>(value);
		update_leaf( p_root, buf, TYPE_INT, (void*)val.Value(), 1 );
	      }
	      break;

	    case (AttributeValueType::STRING):
	      {
		const AttributeValue<MString> &val = dynamic_cast<const AttributeValue<MString> &>(value);
		update_leaf( p_root, buf, TYPE_STRING, (void *)val.Value().CharArray(), 1 );
	      }
	      break;
	    }

	  /* add the row */
	  tsk.m_mutableMatchAnswer->AddNewObject(obj);
	  p_totals[i_goodcol]++;
	  i_chosen[j] = 1;
	}

#ifdef _WIN32
      delete [] i_cols;
#endif
    }

  int i_added = 0;
  for( int i = 0; i < i_totals; i++ )
    i_added += p_totals[i];

  /* do we have enough results ? */
  if( i_added < i_max )
    {
      int i_needed = i_max - i_added;

      /* if not enough results, add remaining ones and update the stats */
      for( int i = 0; (i < dataAnswer.ObjectsCount()) && ( i < i_needed); i++ )
	if( i_chosen[i] == 0 )
	  {
	    const ObjectInstance& obj = dataAnswer.ObjectAtIndex(i);

	    const ObjectInstanceAttribute  *p_instanceAttr = &(obj.AttributeAtIndex( i_goodcol ));
	    FASSERT( IS_INVALID_OBJECT_INSTANCE_ATTRIBUTE( *p_instanceAttr ) == false, "Invalid object instance attribute" );

	    const IAttributeValue &value = p_instanceAttr->Value();

	    char buf[128];
	    sprintf( buf, "%s.%s", p_instanceAttr->Attribute().ParentInterface().TableName().CharArray(), p_instanceAttr->Attribute().ColumnName().CharArray() );

	    switch (value.TypeOf())
	      {
	      case (AttributeValueType::INTEGER):
		{
		  const AttributeValue<int> &val = dynamic_cast<const AttributeValue<int> &>(value);
		  update_leaf( p_root, buf, TYPE_INT, (void*)val.Value(), 1 );
		}
		break;

	      case (AttributeValueType::STRING):
		{
		  const AttributeValue<MString> &val = dynamic_cast<const AttributeValue<MString> &>(value);
		  update_leaf( p_root, buf, TYPE_STRING, (void *)val.Value().CharArray(), 1 );
		}
		break;
	      }

	    tsk.m_mutableMatchAnswer->AddNewObject(dataAnswer.ObjectAtIndex(i));
	  }
    }

#ifdef _WIN32
  delete [] i_chosen;
#endif

  free( p_totals );
}

void Matcher::Process(MatchTask& tsk)
{
  const DataAnswer& dataAnswer = tsk.DataEntityAnswer();
  tsk.m_mutableMatchAnswer = new MutableMatchAnswer();

  Matcher  matcher;

  matcher.Match( tsk, dataAnswer );
}

Matcher::Matcher( void )
{
}

Matcher::~Matcher(void)
{
}
