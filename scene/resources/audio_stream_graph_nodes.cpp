/**************************************************************************/
/*  audio_stream_graph_nodes.cpp                                          */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "audio_stream_graph_nodes.h"

String AudioStreamGraphInputNode::get_caption() const {
	return "AudioInput";
}

int AudioStreamGraphInputNode::get_input_port_count() const {
	return 0;
}

AudioStreamGraphNode::PortType AudioStreamGraphInputNode::get_input_port_type(int p_port) const {
	return PORT_TYPE_STREAM;
}

String AudioStreamGraphInputNode::get_input_port_name(int p_port) const {
	return "";
};

int AudioStreamGraphInputNode::get_output_port_count() const {
	return 1;
}

AudioStreamGraphNode::PortType AudioStreamGraphInputNode::get_output_port_type(int p_port) const {
	return PORT_TYPE_STREAM;
}

String AudioStreamGraphInputNode::get_output_port_name(int p_port) const {
	return "out";
};

Ref<AudioStream> AudioStreamGraphInputNode::pre_process_node() const {
	print_line("hello input node");
	return get_stream();
}

void AudioStreamGraphInputNode::set_stream(Ref<AudioStream> p_stream) {
	if (!p_stream.is_valid()) {
		return;
	}
	stream = p_stream;
}

Ref<AudioStream> AudioStreamGraphInputNode::get_stream() const {
	return stream;
}

bool AudioStreamGraphInputNode::is_show_prop_names() const {
	return true;
}

Vector<StringName> AudioStreamGraphInputNode::get_editable_properties() const {
	Vector<StringName> props;
	props.push_back("stream");

	return props;
}

void AudioStreamGraphInputNode::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_stream", "stream"), &AudioStreamGraphInputNode::set_stream);
	ClassDB::bind_method(D_METHOD("get_stream"), &AudioStreamGraphInputNode::get_stream);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "stream", PROPERTY_HINT_RESOURCE_TYPE, "AudioStream"), "set_stream", "get_stream");
}

String AudioStreamGraphOutputNode::get_caption() const {
	return "Output";
}

int AudioStreamGraphOutputNode::get_input_port_count() const {
	return 1;
}

AudioStreamGraphNode::PortType AudioStreamGraphOutputNode::get_input_port_type(int p_port) const {
	return PORT_TYPE_STREAM;
}

String AudioStreamGraphOutputNode::get_input_port_name(int p_port) const {
	return "input";
};

int AudioStreamGraphOutputNode::get_output_port_count() const {
	return 0;
}

AudioStreamGraphNode::PortType AudioStreamGraphOutputNode::get_output_port_type(int p_port) const {
	return PORT_TYPE_STREAM;
}

String AudioStreamGraphOutputNode::get_output_port_name(int p_port) const {
	return "";
};

Ref<AudioStream> AudioStreamGraphOutputNode::pre_process_node() const {
	List<Ref<AudioStreamGraphNode>> nodes = get_connected_input_nodes();
	if (nodes.size() > 0) {
		return nodes.get(0)->pre_process_node();
	}

	return Ref<AudioStream>();
}

String AudioStreamGraphRandomizerNode::get_caption() const {
	return "Randomizer";
}

int AudioStreamGraphRandomizerNode::get_input_port_count() const {
	return 2;
}

AudioStreamGraphNode::PortType AudioStreamGraphRandomizerNode::get_input_port_type(int p_port) const {
	return PORT_TYPE_STREAM;
}

String AudioStreamGraphRandomizerNode::get_input_port_name(int p_port) const {
	return "input";
};

int AudioStreamGraphRandomizerNode::get_output_port_count() const {
	return 1;
}

AudioStreamGraphNode::PortType AudioStreamGraphRandomizerNode::get_output_port_type(int p_port) const {
	return PORT_TYPE_STREAM;
}

String AudioStreamGraphRandomizerNode::get_output_port_name(int p_port) const {
	return "out";
};

Ref<AudioStream> AudioStreamGraphRandomizerNode::pre_process_node() const {
	List<Ref<AudioStreamGraphNode>> nodes = get_connected_input_nodes();

	const int selected_node = Math::random(0, nodes.size() - 1);

	print_line("hello randomizer node: ", selected_node);
	return nodes.get(selected_node)->pre_process_node();
}