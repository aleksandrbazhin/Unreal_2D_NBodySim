#pragma once

#include "CoreMinimal.h"



/**
 * Node of a Barnes-Hut quad tree
 */
struct BHTreeNode
{
	FVector2D CenterOfMass;
	FVector2D BottomLeft;
	FVector2D TopRight;

	float TotalMass;
	BHTreeNode* TLNode;
	BHTreeNode* TRNode;
	BHTreeNode* BLNode;
	BHTreeNode* BRNode;
};


/**
 * Quad tree used by Barnes-Hut algorithm
 */
class NBODYSIM_API BarnesHutTree
{
public:
	BarnesHutTree();
	~BarnesHutTree();
	float Theta = 0.7;
private:
	BHTreeNode* Root;
};
