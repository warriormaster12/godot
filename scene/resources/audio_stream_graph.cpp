/**************************************************************************/
/*  audio_stream_graph.cpp                                               */
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

#include "audio_stream_graph.h"

void AudioStreamGraph::_bind_methods() {
}

void AudioStreamGraph::add_node(const StringName &p_name, Ref<AudioStreamGraphNode> p_node, const Vector2 &p_position) {
	ERR_FAIL_COND(nodes.has(p_name));
	ERR_FAIL_COND(p_node.is_null());
	ERR_FAIL_COND(p_name == SceneStringName(output));
	ERR_FAIL_COND(String(p_name).contains("/"));

	Node n;
	n.node = p_node;
	n.position = p_position;
	//n.connections.resize(n.node->get_input_count());
	nodes[p_name] = n;

	emit_changed();
	emit_signal(SNAME("tree_changed"));

	// p_node->connect(SNAME("tree_changed"), callable_mp(this, &AnimationNodeBlendTree::_tree_changed), CONNECT_REFERENCE_COUNTED);
	// p_node->connect(SNAME("animation_node_renamed"), callable_mp(this, &AnimationNodeBlendTree::_animation_node_renamed), CONNECT_REFERENCE_COUNTED);
	// p_node->connect(SNAME("animation_node_removed"), callable_mp(this, &AnimationNodeBlendTree::_animation_node_removed), CONNECT_REFERENCE_COUNTED);
	// p_node->connect_changed(callable_mp(this, &AnimationNodeBlendTree::_node_changed).bind(p_name), CONNECT_REFERENCE_COUNTED);
}

Ref<AudioStreamGraphNode> AudioStreamGraph::get_node(const StringName &p_name) const {
	ERR_FAIL_COND_V(!nodes.has(p_name), Ref<AudioStreamGraphNode>());

	return nodes[p_name].node;
}

void AudioStreamGraph::remove_node(const StringName &p_name) {
	ERR_FAIL_COND(!nodes.has(p_name));
	ERR_FAIL_COND(p_name == SceneStringName(output)); //can't delete output

	{
		Ref<AudioStreamGraphNode> node = nodes[p_name].node;
		// node->disconnect(SNAME("tree_changed"), callable_mp(this, &AnimationNodeBlendTree::_tree_changed));
		// node->disconnect(SNAME("animation_node_renamed"), callable_mp(this, &AnimationNodeBlendTree::_animation_node_renamed));
		// node->disconnect(SNAME("animation_node_removed"), callable_mp(this, &AnimationNodeBlendTree::_animation_node_removed));
		// node->disconnect_changed(callable_mp(this, &AnimationNodeBlendTree::_node_changed));
	}

	nodes.erase(p_name);

	// Erase connections to name.
	for (KeyValue<StringName, Node> &E : nodes) {
		for (int i = 0; i < E.value.connections.size(); i++) {
			if (E.value.connections[i] == p_name) {
				E.value.connections.write[i] = StringName();
			}
		}
	}

	// emit_signal(SNAME("animation_node_removed"), get_instance_id(), p_name);
	// emit_changed();
	// emit_signal(SNAME("tree_changed"));
}

void AudioStreamGraph::rename_node(const StringName &p_name, const StringName &p_new_name) {
	ERR_FAIL_COND(!nodes.has(p_name));
	ERR_FAIL_COND(nodes.has(p_new_name));
	ERR_FAIL_COND(p_name == SceneStringName(output));
	ERR_FAIL_COND(p_new_name == SceneStringName(output));

	//nodes[p_name].node->disconnect_changed(callable_mp(this, &AnimationNodeBlendTree::_node_changed));

	nodes[p_new_name] = nodes[p_name];
	nodes.erase(p_name);

	// Rename connections.
	for (KeyValue<StringName, Node> &E : nodes) {
		for (int i = 0; i < E.value.connections.size(); i++) {
			if (E.value.connections[i] == p_name) {
				E.value.connections.write[i] = p_new_name;
			}
		}
	}
	// Connection must be done with new name.
	//nodes[p_new_name].node->connect_changed(callable_mp(this, &AnimationNodeBlendTree::_node_changed).bind(p_new_name), CONNECT_REFERENCE_COUNTED);

	emit_signal(SNAME("animation_node_renamed"), get_instance_id(), p_name, p_new_name);
	emit_signal(SNAME("tree_changed"));
}

bool AudioStreamGraph::has_node(const StringName &p_name) const {
	return nodes.has(p_name);
}

void AudioStreamGraph::get_node_list(List<StringName> *r_list) {
	for (const KeyValue<StringName, Node> &E : nodes) {
		r_list->push_back(E.key);
	}
}

AudioStreamGraph::AudioStreamGraph() {
}

void AudioStreamGraphNode::_bind_methods() {
}

AudioStreamGraphNode::AudioStreamGraphNode() {
}