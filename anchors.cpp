#include "anchors.h"

using namespace std;

float calculate_scale(float min_scale, float max_scale, int stride_index, int num_strides)
{
	float scale = min_scale + (max_scale - min_scale) * stride_index / (num_strides - 1.0);
	return scale;
}

void generate_anchors(std::vector<anchorType>& anchors)
{
	/* generate 2944 anchors */
	/*int num_layers = 5;
	float min_scale = 0.1171875, max_scale = 0.75;
	int input_size_height = 256, input_size_width = 256;
	float anchor_offset_x = 0.5, anchor_offset_y = 0.5;
	int strides[5] = { 8, 16, 32, 32, 32 };
	float aspect_ratios = 1.0;
	bool reduce_boxes_in_lowest_layer = false;
	float interpolated_scale_aspect_ratio = 1.0;
	bool fixed_anchor_size = true;*/
	/* generate 896 anchors */
	int num_layers = 4;
	float min_scale = 0.1484375, max_scale = 0.75;
	int input_size_height = 128, input_size_width = 128;
	float anchor_offset_x = 0.5, anchor_offset_y = 0.5;
	int strides[4] = {8, 16, 16, 16};
	float aspect_ratios = 1.0;
	bool reduce_boxes_in_lowest_layer = false;
	float interpolated_scale_aspect_ratio = 1.0;
	bool fixed_anchor_size = true;

	int layer_id = 0;
	int last_same_stride_layer;
	int strides_size = sizeof(strides) / sizeof(strides[0]);
	while (layer_id < strides_size)
	{
		std::vector<float> v_aspect_ratios;
		std::vector<float> v_scales;
		std::vector<float> anchor_height;
		std::vector<float> anchor_width;
		//for same stride, we merge the anchors to the same order
		last_same_stride_layer = layer_id;
		while ((last_same_stride_layer < strides_size) && (strides[last_same_stride_layer] == strides[layer_id]))
		{
			float scale = calculate_scale(min_scale, max_scale, last_same_stride_layer, strides_size);
			if (last_same_stride_layer == 0 && reduce_boxes_in_lowest_layer)
			{
				v_aspect_ratios.push_back(1.0);
				v_aspect_ratios.push_back(2.0);
				v_aspect_ratios.push_back(0.5);
				v_scales.push_back(0.1);
				v_scales.push_back(scale);
				v_scales.push_back(scale);
			}
			else {
				v_aspect_ratios.push_back(1.0);
				v_scales.push_back(scale);
				float scale_next;
				if (last_same_stride_layer == strides_size - 1)
				{
					scale_next = 1.0;
				}
				else {
					scale_next = calculate_scale(min_scale, max_scale, last_same_stride_layer + 1, strides_size);
				}
				v_scales.push_back(sqrt(scale*scale_next));
				v_aspect_ratios.push_back(interpolated_scale_aspect_ratio);
			}
			last_same_stride_layer++;
		}
		for (int i = 0; i < v_aspect_ratios.size(); i++)
		{
			float ratio_sqrt = sqrt(v_aspect_ratios[i]);
			anchor_height.push_back(v_scales[i] / ratio_sqrt);
			anchor_width.push_back(v_scales[i] * ratio_sqrt);
		}
		int stride = strides[layer_id];
		int feature_map_height = int(ceil(input_size_height / stride));
		int feature_map_width = int(ceil(input_size_width / stride));
		for(int y = 0; y < feature_map_height; y++)
		{
			for(int x = 0; x < feature_map_width; x++)
			{
				for (int anchor_id = 0; anchor_id < anchor_height.size(); anchor_id++)
				{
					float x_center = (x + anchor_offset_x) / feature_map_width;  // anchor_offset_x =0.5
					float y_center = (y + anchor_offset_y) / feature_map_height;
					float _width, _height;
					if (fixed_anchor_size)
					{
						_width = 1.0;
						_height = 1.0;
					}
					else {
						_width = anchor_width[anchor_id];
						_height = anchor_height[anchor_id];
					}
					anchors.push_back(anchorType{ x_center, y_center, _width, _height });
				}
			}
		}
		layer_id = last_same_stride_layer;
	}
}


