#pragma once
# include <vector>

typedef struct anchorType {
	float x;
	float y;
	float w;
	float h;
} anchorType;

void generate_anchors(std::vector<anchorType>& anchors);
