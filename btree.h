#pragma once
#include <cerrno>
#include <fcntl.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include "constants.h"



NodeType get_node_type(void *node)
{
  uint8_t value = *(static_cast<uint8_t *>(node) + NODE_TYPE_OFFSET);
  return static_cast<NodeType>(value);
}

void set_node_type(void *node, NodeType type)
{
  uint8_t value = static_cast<uint8_t>(type);
  uint8_t *ptr = static_cast<uint8_t *>(node) + NODE_TYPE_OFFSET;
  *(ptr) = value;
}

bool is_node_root(void *node)
{
  uint8_t value = *(static_cast<uint8_t *>(node) + IS_ROOT_OFFSET);
  return static_cast<bool>(value);
}

void set_node_root(void *node, bool is_root)
{
  uint8_t value = static_cast<uint8_t>(is_root);
  uint8_t *ptr = static_cast<uint8_t *>(node) + IS_ROOT_OFFSET;
  *(ptr) = value;
}

uint32_t *node_parent(void *node)
{
  return reinterpret_cast<uint32_t *>(static_cast<uint8_t *>(node) + PARENT_POINTER_OFFSET);
}

uint32_t *internal_node_num_keys(void *node)
{
  return reinterpret_cast<uint32_t *>(static_cast<uint8_t *>(node) + INTERNAL_NODE_NUM_KEYS_OFFSET);
}

uint32_t *internal_node_right_child(void *node)
{
  return reinterpret_cast<uint32_t *>(static_cast<uint8_t *>(node) + INTERNAL_NODE_RIGHT_CHILD_OFFSET);
}

uint32_t *internal_node_cell(void *node, uint32_t cell_num)
{
  return reinterpret_cast<uint32_t *>(static_cast<uint8_t *>(node) + INTERNAL_NODE_HEADER_SIZE + cell_num * INTERNAL_NODE_CELL_SIZE);
}

uint32_t *internal_node_child(void *node, uint32_t child_num)
{
  uint32_t num_keys = *internal_node_num_keys(node);
  if (child_num > num_keys)
  {
    printf("Tried to access child_num %d > num_keys %d\n", child_num, num_keys);
    exit(EXIT_FAILURE);
  }
  else if (child_num == num_keys)
  {
    uint32_t *right_child = internal_node_right_child(node);
    if (*right_child == INVALID_PAGE_NUM)
    {
      printf("Tried to access right child of node, but was invalid page\n");
      exit(EXIT_FAILURE);
    }
    return right_child;
  }
  else
  {
    uint32_t *child = internal_node_cell(node, child_num);
    if (*child == INVALID_PAGE_NUM)
    {
      printf("Tried to access child %d of node, but was invalid page\n", child_num);
      exit(EXIT_FAILURE);
    }
    return child;
  }
}

uint32_t *internal_node_key(void *node, uint32_t key_num)
{
  return reinterpret_cast<uint32_t *>(reinterpret_cast<char *>(internal_node_cell(node, key_num)) + INTERNAL_NODE_CHILD_SIZE);
}

uint32_t *leaf_node_num_cells(void *node)
{
  return reinterpret_cast<uint32_t *>(static_cast<uint8_t *>(node) + LEAF_NODE_NUM_CELLS_OFFSET);
}

uint32_t *leaf_node_next_leaf(void *node)
{
  return reinterpret_cast<uint32_t *>(static_cast<uint8_t *>(node) + LEAF_NODE_NEXT_LEAF_OFFSET);
}

void *leaf_node_cell(void *node, uint32_t cell_num)
{
  return static_cast<void *>(static_cast<uint8_t *>(node) + LEAF_NODE_HEADER_SIZE + cell_num * LEAF_NODE_CELL_SIZE);
}

uint32_t *leaf_node_key(void *node, uint32_t cell_num)
{
  return reinterpret_cast<uint32_t *>(leaf_node_cell(node, cell_num));
}

void *leaf_node_value(void *node, uint32_t cell_num)
{
  return static_cast<void *>(static_cast<uint8_t *>(leaf_node_cell(node, cell_num)) + LEAF_NODE_KEY_SIZE);
}
