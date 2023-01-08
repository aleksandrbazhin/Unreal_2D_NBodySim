// Fill out your copyright notice in the Description page of Project Settings.


#include "BarnesHutTree.h"


void BarnesHutTree::AddMass(float Mass, FVector2D Position)
{
    BHTreeNode* node = Root.Get();
    node->BodiesCount += 1;
    node->TotalMass += Mass;
    
    while (node->BodiesCount > 4) {
        node = node->GetQuadrant(Position);
        node->BodiesCount += 1;
        node->TotalMass += Mass;
        node->CenterOfMass = Position;
    }
}