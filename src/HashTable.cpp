template <typename D>
bool HashTable<D>::add(const D & data)
{
  // add the entry to the table
  int index = hash(data);

  if (! table[index].empty() )
    {
      // this is just a performance check
      // cerr << "Warning: collision" << endl;
    }

  itemsByNumber.add(data);

  if ( searchList( table[index], data ) != -1 )
    {
      return false;
    }

  table[ index ].push_back( Node(data, numElements() ));
  numberOfElements++;

  return true;
}

template <typename D>
int HashTable<D>::lookup(const D & data) const
{
  int index = hash(data);
  
  return searchList( table[index], data );
}

template <typename D>
int HashTable<D>::searchList(const list<Node> & li, const D & data) const
{
  typename list<Node >::const_iterator iter;

  for (iter = li.begin(); iter != li.end(); iter++)
    {
      if ( iter->data == data )
	{
	  return iter->code;
	}
    }

  return -1;
}

template <typename D>
unsigned int HashTable<D>::hash(const D & data) const
{
  return defined_hash(data) % table.size();
}

template <typename D>
const D & HashTable<D>::getItem(int num)
{
  return itemsByNumber[num];
}

