#pragma once

#include "CoreMinimal.h"



/**
 * Node of a Barnes-Hut quad tree
 */
struct BHTreeNode
{
	BHTreeNode(FVector2D MinPosition, FVector2D MaxPosition): BottomLeft(MinPosition), TopRight(MaxPosition) {};
	// BHTreeNode() {};
	// ~BHTreeNode() {};
	FVector2D BottomLeft;
	FVector2D TopRight;
	FVector2D CenterOfMass;
	float TotalMass = 0;
	// BHTreeNode* TLNode;
	// BHTreeNode* TRNode;
	// BHTreeNode* BLNode;
	// BHTreeNode* BRNode;
	TUniquePtr<BHTreeNode> TRNode = NULL; 
	TUniquePtr<BHTreeNode> BRNode = NULL;
	TUniquePtr<BHTreeNode> TLNode = NULL;
	TUniquePtr<BHTreeNode> BLNode = NULL;

	int BodiesCount = 0;

	BHTreeNode* GetQuadrant(const FVector2D& Position) {
		FVector2D center = (TopRight - BottomLeft) * 0.5;
		if (Position.X > center.X) {
			if (Position.Y > center.Y) {
				if (!TRNode.IsValid()) {
					TRNode = MakeUnique<BHTreeNode>(center, TopRight);
				}
				return TRNode.Get();
			} else {
				if (!BRNode.IsValid()) {
					BRNode = MakeUnique<BHTreeNode>(FVector2D(center.X, BottomLeft.Y), FVector2D(TopRight.X, center.Y));
				}
				return BRNode.Get();
			}
		} else {
			if (Position.Y > center.Y) {
				if (!TLNode.IsValid()) {
					TLNode = MakeUnique<BHTreeNode>(FVector2D(BottomLeft.X, center.Y), FVector2D(center.X, TopRight.Y));
				}
				return TLNode.Get();
			} else {
				if (!BLNode.IsValid()) {
					BLNode = MakeUnique<BHTreeNode>(BottomLeft, center);
				}
				return BLNode.Get();
			}
		}
	};
};


/**
 * Quad tree used by Barnes-Hut algorithm
 */
class NBODYSIM_API BarnesHutTree
{
public:
	BarnesHutTree() = default;
	BarnesHutTree(FVector2D MinPosition, FVector2D MaxPosition):
		Root(MakeUnique<BHTreeNode>(MinPosition, MaxPosition)) {};
	void AddMass(float Mass, FVector2D Position);
	// ~BarnesHutTree();
	float Theta = 0.7;
	
	int getTotalBodies() {
		return Root->BodiesCount;
	}
private:
	TUniquePtr<BHTreeNode> Root;
};
