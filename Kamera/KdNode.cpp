#include "KdNode.h"
#include <iostream>
#include <algorithm>
#include "HitResult.h"

KdNode::KdNode(std::vector<KdPrimitive*>& tris) : left_(nullptr), right_(nullptr), triangles_(tris)
{
	//Leere Node
	if (tris.size() == 0)
	{
		std::cerr << "Created empty node" << std::endl;
		return;
	}

	//Perfect
	if (tris.size() == 1)
	{
		bbox_ = triangles_[0]->GetBoundingBox();
		return;
	}

	bbox_ = BoundingBox(tris);

	//Abbruchbedingung
	if (tris.size() <= 12)
		return;

	int longestAxis = bbox_.GetLongestAxis();
	std::vector<KdPrimitive*> leftTris;
	std::vector<KdPrimitive*> rightTris;
	for (int i = 0; i < 3; ++i)
	{
		SplitByAxis(tris, (longestAxis + i) % 3, leftTris, rightTris);

		//if split didn't work --> use other axis
		if (leftTris.size() > 0 && rightTris.size() > 0)
			break;

		leftTris.clear();
		rightTris.clear();
	}

	//SplitByAxis(tris, longestAxis, leftTris, rightTris);
	 
	if (leftTris.size() == 0 || rightTris.size() == 0)
	return;

	left_ = new KdNode(leftTris);
	right_ = new KdNode(rightTris);
}

KdNode::~KdNode()
{
}

void KdNode::SplitByAxis(std::vector<KdPrimitive*>& tris, int axis, std::vector<KdPrimitive*>& leftTris, std::vector<KdPrimitive*>& rightTris)
{
	auto median = tris.begin() + tris.size() / 2;
	//Calculate median
	std::nth_element(tris.begin(), median, tris.end(), [&](const KdPrimitive* t1, const KdPrimitive* t2)
	{
		return t1->GetBoundingBox().GetMax()[axis] < t2->GetBoundingBox().GetMax()[axis];
		//return t1->GetCenter()[axis] < t2->GetCenter()[axis];
	});

	float split = (*median)->GetBoundingBox().GetMax()[axis];
	//float split = (*median)->GetCenter()[axis];

	for (auto it = tris.begin(); it != tris.end(); ++it)
	{
		if ((*it)->GetBoundingBox().GetMax()[axis] < split)
		//if ((*it)->GetCenter()[axis] < split)
			leftTris.push_back(*it);
		else
			rightTris.push_back(*it);
	}
}

bool KdNode::IsHit(HitResult& result, Ray& ray) const
{
	if (!bbox_.IsHit(result, ray))
		return false;

	// Child nodes present
	if (left_ != nullptr || right_ != nullptr)
	{
		HitResult leftResult, rightResult;
		if (left_->IsHit(leftResult, ray) || right_->IsHit(rightResult, ray))
		{
			if (leftResult.Distance < rightResult.Distance)
				result = leftResult;
			else
				result = rightResult;

			return true;
		}

		return false;
	}

	// Leaf node --> check tris
	HitResult tmpResult;
	bool isHit = false;
	for (auto it = triangles_.begin(); it != triangles_.end(); ++it)
	{
		if (!(*it)->IsHit(tmpResult, ray))
			continue;

		isHit = true;
		if (result.Distance > tmpResult.Distance)
		{
			result.Distance = tmpResult.Distance;
			result.Triangle = *it;
		}
	}
	return isHit;
}

void KdNode::RenderLeafs(Shader& shader, int depth) const
{
	if (left_ != nullptr && right_ != nullptr)
	{
		left_->RenderLeafs(shader, depth + 1);
		right_->RenderLeafs(shader, depth + 1);
	}
	else
	{
		bbox_.Render(shader, depth);
	}
}

void KdNode::RenderLeafs(Shader& shader, int desiredDepth, int depth) const
{
	if (desiredDepth == depth || (left_ == nullptr || right_ == nullptr))
	{
		bbox_.Render(shader, depth);
	}
	else 
	{
		left_->RenderLeafs(shader, desiredDepth, depth + 1);
		right_->RenderLeafs(shader, desiredDepth, depth + 1);
	}
}
