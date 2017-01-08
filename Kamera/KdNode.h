#pragma once
#include "KdPrimitive.h"
#include <vector>
#include "BoundingBox.h"

class Shader;

class KdNode
{
public:
	KdNode(std::vector<KdPrimitive*>& tris);
	~KdNode();

	void SplitByAxis(std::vector<KdPrimitive*>& tris, int longestAxis, std::vector<KdPrimitive*>& leftTris, std::vector<KdPrimitive*>& rightTris);
	bool IsHit(HitResult& result, Ray& ray) const;
	
	void RenderLeafs(Shader& shader, int depth) const;
	void RenderLeafs(Shader& shader, int desiredDepth, int depth) const;

protected:
	KdNode* left_;
	KdNode* right_;
	BoundingBox bbox_;
	std::vector<KdPrimitive*> triangles_;

};

