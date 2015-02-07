#include "OctTree.h"

const int MAX_NUM_OF_CHILDREN = 4;

OctTree::OctTree()
{
	size.w = 1000000.0;
	size.h = 1000000.0;
	size.d = 1000000.0;
	amountOfChildren = 0;
	nodes = NULL;
	data = NULL;
}

OctTree::OctTree( double x, double y, double z, double w, double h, double d )
	:position( x, y, z), size( w, h, d )
{
	size.w = 1000000.0;
	size.h = 1000000.0;
	size.d = 1000000.0;
	amountOfChildren = 0;
	nodes = NULL;
	data = NULL;
}

bool OctTree::InBounds( Point3D position, Size3D size, Point3D point )
{
	double left = position.x - size.w / 2.0;
	double right = position.x + size.w / 2.0;
	double top = position.y + size.h / 2.0;
	double bottom = position.y - size.h / 2.0;
	double back = position.z - size.d / 2.0;
	double front = position.z + size.d / 2.0;

	if( point.x >= left		&&	point.x <= right &&
		point.y >= bottom	&&	point.y <= top   &&
		point.z >= back		&&	point.z <= front   )
	{
		return true;
	}

	return false;
}

bool OctTree::OverlapArea( Point3D positionA, Size3D sizeA, Point3D positionB, Size3D sizeB )
{
	double leftA = positionA.x - sizeA.w / 2.0;
	double rightA = positionA.x + sizeA.w / 2.0;
	double topA = positionA.y + sizeA.h / 2.0;
	double bottomA = positionA.y - sizeA.h / 2.0;
	double backA = positionA.z - sizeA.d / 2.0;
	double frontA = positionA.z + sizeA.d / 2.0;

	double leftB = positionB.x - sizeB.w / 2.0;
	double rightB = positionB.x + sizeB.w / 2.0;
	double topB = positionB.y + sizeB.h / 2.0;
	double bottomB = positionB.y - sizeB.h / 2.0;
	double backB = positionB.z - sizeB.d / 2.0;
	double frontB = positionB.z + sizeB.d / 2.0;

	if( leftA  < rightB )
		return true;

	if( rightA > leftB )
		return true;

	if( topA > bottomB )
		return true;

	if( bottomA < topB )
		return true;

	if( backA < frontB )
		return true;

	if( frontA > backB )
		return true;

	return false;
}

void OctTree::Split()
{

	double left = position.x - size.w / 2.0;
	double right = position.x + size.w / 2.0;
	double top = position.y + size.h / 2.0;
	double bottom = position.y - size.h / 2.0;
	double back = position.z - size.d / 2.0;
	double front = position.z + size.d / 2.0;

	double halfWidth = size.w / 2.0;
	double halfHeight = size.h / 2.0;
	double halfDepth = size.d / 2.0;

	double quarterWidth = size.w / 4.0;
	double quarterHeight = size.h / 4.0;
	double quarterDepth = size.d / 4.0;

	nodes = new OctTree[8];

// Top
	//1
	nodes[0].position.x = left + quarterWidth;
	nodes[0].position.y = top - quarterHeight;
	nodes[0].position.z = front - quarterDepth;
	nodes[0].size.w = halfWidth;
	nodes[0].size.h = halfHeight;
	nodes[0].size.d = halfDepth;
	//2
	nodes[1].position.x = left + quarterWidth;
	nodes[1].position.y = top - quarterHeight;
	nodes[1].position.z = back + quarterDepth;
	nodes[1].size.w = halfWidth;
	nodes[1].size.h = halfHeight;
	nodes[1].size.d = halfDepth;
	//3
	nodes[2].position.x = right - quarterWidth;
	nodes[2].position.y = top - quarterHeight;
	nodes[2].position.z = back + quarterDepth;
	nodes[2].size.w = halfWidth;
	nodes[2].size.h = halfHeight;
	nodes[2].size.d = halfDepth;
	//4
	nodes[3].position.x = right - quarterWidth;
	nodes[3].position.y = top - quarterHeight;
	nodes[3].position.z = front - quarterDepth;
	nodes[3].size.w = halfWidth;
	nodes[3].size.h = halfHeight;
	nodes[3].size.d = halfDepth;
// Bottom
	//1
	nodes[4].position.x = left + quarterWidth;
	nodes[4].position.y = bottom + quarterHeight;
	nodes[4].position.z = front - quarterDepth;
	nodes[4].size.w = halfWidth;
	nodes[4].size.h = halfHeight;
	nodes[4].size.d = halfDepth;
	//2
	nodes[5].position.x = left + quarterWidth;
	nodes[5].position.y = bottom + quarterHeight;
	nodes[5].position.z = back + quarterDepth;
	nodes[5].size.w = halfWidth;
	nodes[5].size.h = halfHeight;
	nodes[5].size.d = halfDepth;
	//3
	nodes[6].position.x = right - quarterWidth;
	nodes[6].position.y = bottom + quarterHeight;
	nodes[6].position.z = back + quarterDepth;
	nodes[6].size.w = halfWidth;
	nodes[6].size.h = halfHeight;
	nodes[6].size.d = halfDepth;
	//4
	nodes[7].position.x = right - quarterWidth;
	nodes[7].position.y = bottom + quarterHeight;
	nodes[7].position.z = front - quarterDepth;
	nodes[7].size.w = halfWidth;
	nodes[7].size.h = halfHeight;
	nodes[7].size.d = halfDepth;
}

bool OctTree::Insert( WorldObject* data )
{
	// If not in the tree, exit
	if( !InBounds( position, size, data->GetPosition() ) )
		return false;

	length++;

	// If there is no more room in the node, split
	if( amountOfChildren == MAX_NUM_OF_CHILDREN )
	{
		// Create the children nodes if required
		if( nodes == NULL )
			Split();

		// ping each node for entry
		if( nodes[0].Insert( data ) ) { return true; }
		if( nodes[1].Insert( data ) ) { return true; }
		if( nodes[2].Insert( data ) ) { return true; }
		if( nodes[3].Insert( data ) ) { return true; }
		if( nodes[4].Insert( data ) ) { return true; }
		if( nodes[5].Insert( data ) ) { return true; }
		if( nodes[6].Insert( data ) ) { return true; }
		if( nodes[7].Insert( data ) ) { return true; }
	}
	else
	{
		if( this->data == NULL )
		{
			this->data = new WorldObject*[MAX_NUM_OF_CHILDREN];
		}

		// Insert data into this node
		this->data[amountOfChildren] = data;
		amountOfChildren++;
		return true;
	}

	// Should never get here
	return false;
}

std::vector<WorldObject*> OctTree::GetObjects( Point3D position, Size3D size )
{
	// Vector
	std::vector<WorldObject*> dataVect;

	// If the position is outside this node, exit
	if( !OverlapArea( this->position, this->size, position, size ) )
		return dataVect;

	// Cycle through data for applicable data
	for( int i = 0; i < amountOfChildren; i++ )
	{
		if( InBounds( position, size, data[i]->GetPosition() ) )
			dataVect.push_back( data[i] );
	}

	// If this node has children, do the same for each child
	if( nodes != NULL )
	{
		for( int i = 0; i < 8; i++ )
		{
			std::vector<WorldObject*> childDataVect = nodes[i].GetObjects( position, size );
			dataVect.insert( dataVect.end(), childDataVect.begin(), childDataVect.end() );
		}
	}

	return dataVect;
}

void OctTree::clear()
{
	delete[] data;
	data = NULL;

	if( nodes != NULL )
	{
		for( int i = 0; i < 8; i++ )
		{
			nodes[i].clear();
		}
		delete[] nodes;
	}

	amountOfChildren = 0;
}