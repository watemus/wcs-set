//
// Created by watemus on 02.02.2022.
//

#include <cstdint>
#include <cstdlib>
#include <list>

#ifndef RBTREE_H
#define RBTREE_H

template<typename ValueType>
class Set {
private:
  static const int8_t kBlack = 0;
  static const int8_t kRed = 1;

  class Node {
  private:
    ValueType value_;
    Node *parent_;
    Node *left_;
    Node *right_;
    int8_t color_;
  public:
    Node(const ValueType &elem, int8_t color)
        : value_(elem),
          parent_(nullptr),
          left_(nullptr),
          right_(nullptr),
          color_(color) {}

    Node(const Node &other)
        : value_(other.value_),
          parent_(nullptr),
          left_(nullptr),
          right_(nullptr),
          color_(other.color_) {}

    inline Node *GetParent() const {
      return parent_;
    }

    inline void SetParent(Node *parent) {
      parent_ = parent;
    }

    inline Node *GetLeft() const {
      return left_;
    }

    inline void SetLeft(Node *left) {
      left_ = left;
    }

    inline Node *GetRight() const {
      return right_;
    }

    inline void SetRight(Node *right) {
      right_ = right;
    }

    inline int8_t GetColor() const {
      return color_;
    }

    inline void SetColor(int8_t color) {
      color_ = color;
    }

    inline const ValueType &GetValue() const {
      return value_;
    }

    ~Node() {
      delete right_;
      delete left_;
    }
  };

  Node *root_ = nullptr;
  Node *node_begin_ = nullptr;
  Node *node_end_ = nullptr;
  size_t size_ = 0;

  inline static Node *GetParent(Node *root) {
    if (root) {
      return root->GetParent();
    }
    return nullptr;
  }

  inline static void SetParent(Node *root, Node *parent) {
    if (root) {
      root->SetParent(parent);
    }
  }

  inline static Node *GetLeft(Node *root) {
    if (root) {
      return root->GetLeft();
    }
    return nullptr;
  }

  inline static void SetLeft(Node *root, Node *child) {
    if (root) {
      root->SetLeft(child);
      SetParent(GetLeft(root), root);
    }
  }

  inline static Node *GetRight(Node *root) {
    if (root) {
      return root->GetRight();
    }
    return nullptr;
  }

  inline static void SetRight(Node *root, Node *child) {
    if (root) {
      root->SetRight(child);
      SetParent(GetRight(root), root);
    }
  }

  inline static int8_t GetColor(Node *root) {
    if (root) {
      return root->GetColor();
    }
    return kBlack;
  }

  inline static void SetColor(Node *root, int8_t color) {
    if (root) {
      root->SetColor(color);
    }
  }

  inline static Node *TakeLeft(Node *root) {
    if (root) {
      auto res = GetLeft(root);
      SetParent(res, nullptr);
      SetLeft(root, nullptr);
      return res;
    }
    return nullptr;
  }

  inline static Node *TakeRight(Node *root) {
    if (root) {
      auto res = GetRight(root);
      SetParent(res, nullptr);
      SetRight(root, nullptr);
      return res;
    }
    return nullptr;
  }

  static Node *CopyTree(Node *root) {
    if (!root) {
      return nullptr;
    }
    auto new_root = new Node(*root);
    SetLeft(new_root, CopyTree(GetLeft(root)));
    SetRight(new_root, CopyTree(GetRight(root)));
    return new_root;
  }

  inline static void LeftRotate(Node *root) {
    auto parent = GetParent(root);
    auto right = TakeRight(root);
    auto right_left = TakeLeft(right);
    SetRight(root, right_left);
    SetLeft(right, root);
    if (GetLeft(parent) == root) {
      SetLeft(parent, right);
    } else {
      SetRight(parent, right);
    }
  }

  inline static void RightRotate(Node *root) {
    auto parent = GetParent(root);
    auto left = TakeLeft(root);
    auto left_right = TakeRight(left);
    SetLeft(root, left_right);
    SetRight(left, root);
    if (GetLeft(parent) == root) {
      SetLeft(parent, left);
    } else {
      SetRight(parent, left);
    }
  }

  void Insert(Node *node) {
    Node *prev = nullptr;
    Node *cur = root_;
    while (cur != nullptr) {
      prev = cur;
      if (node->GetValue() < cur->GetValue()) {
        cur = GetLeft(cur);
      } else {
        cur = GetRight(cur);
      }
    }
    if (!prev) {
      root_ = node;
    } else {
      if (node->GetValue() < prev->GetValue()) {
        SetLeft(prev, node);
      } else {
        SetRight(prev, node);
      }
    }
    InsertFixUp(node);
  }

  void InsertFixUp(Node *node) {
    while (GetColor(GetParent(node)) == kRed) {
      if (GetParent(node) == GetLeft(GetParent(GetParent(node)))) {
        auto uncle = GetRight(GetParent(GetParent(node)));
        if (GetColor(uncle) == kRed) {
          SetColor(GetParent(node), kBlack);
          SetColor(uncle, kBlack);
          SetColor(GetParent(GetParent(node)), kRed);
          node = GetParent(GetParent(node));
        } else if (node == GetRight(GetParent(node))) {
          node = GetParent(node);
          LeftRotate(node);
        } else {
          SetColor(GetParent(node), kBlack);
          SetColor(GetParent(GetParent(node)), kRed);
          RightRotate(GetParent(GetParent(node)));
        }
      } else {
        auto uncle = GetLeft(GetParent(GetParent(node)));
        if (GetColor(uncle) == kRed) {
          SetColor(GetParent(node), kBlack);
          SetColor(uncle, kBlack);
          SetColor(GetParent(GetParent(node)), kRed);
          node = GetParent(GetParent(node));
        } else if (node == GetLeft(GetParent(node))) {
          node = GetParent(node);
          RightRotate(node);
        } else {
          SetColor(GetParent(node), kBlack);
          SetColor(GetParent(GetParent(node)), kRed);
          LeftRotate(GetParent(GetParent(node)));
        }
      }
    }
    while (GetParent(node) != nullptr) {
      node = GetParent(node);
    }
    root_ = node;
    SetColor(root_, kBlack);
  }

  void Transplant(Node *u, Node *v) {
    if (GetParent(u) == nullptr) {
      root_ = v;
      SetParent(v, nullptr);
    } else if (u == GetLeft(GetParent(u))) {
      SetLeft(GetParent(u), v);
    } else {
      SetRight(GetParent(u), v);
    }
  }

  inline static Node *GetTreeMin(Node *root) {
    while (GetLeft(root)) {
      root = GetLeft(root);
    }
    return root;
  }

  inline static Node *GetTreeMax(Node *root) {
    while (GetRight(root)) {
      root = GetRight(root);
    }
    return root;
  }

  void Erase(Node *z) {
    auto y = z;
    auto y_orig_color = GetColor(y);
    Node *x = nullptr;
    if (GetLeft(z) == nullptr) {
      x = GetRight(z);
      Transplant(z, GetRight(z));
    } else if (GetRight(z) == nullptr) {
      x = GetLeft(z);
      Transplant(z, GetLeft(z));
    } else {
      y = GetTreeMin(GetRight(z));
      y_orig_color = GetColor(y);
      x = GetRight(y);
      if (GetParent(y) == z) {
        SetParent(x, y);
      } else {
        Transplant(y, GetRight(y));
        SetRight(y, GetRight(z));
      }
      Transplant(z, y);
      SetLeft(y, GetLeft(z));
      SetColor(y, GetColor(z));
    }
    if (y_orig_color == kBlack) {
      EraseFixUp(x);
    }
  }

  void EraseFixUp(Node *node) {
    while (GetParent(node) != nullptr && GetColor(node) == kBlack) {
      if (node == GetLeft(GetParent(node))) {
        auto brother = GetRight(GetParent(node));
        if (GetColor(brother) == kRed) {
          SetColor(brother, kBlack);
          SetColor(GetParent(node), kRed);
          LeftRotate(GetParent(node));
          brother = GetRight(GetParent(node));
        }
        if (GetColor(GetLeft(brother)) == kBlack && GetColor(GetRight(brother)) == kBlack) {
          SetColor(brother, kRed);
          node = GetParent(node);
        } else if (GetColor(GetRight(brother)) == kBlack) {
          SetColor(GetLeft(brother), kBlack);
          SetColor(brother, kRed);
          RightRotate(brother);
          brother = GetRight(GetParent(node));
        } else {
          SetColor(brother, GetColor(GetParent(node)));
          SetColor(GetParent(node), kBlack);
          SetColor(GetRight(brother), kBlack);
          LeftRotate(GetParent(node));
          while (GetParent(node) != nullptr) {
            node = GetParent(node);
          }
        }
      } else {
        auto brother = GetLeft(GetParent(node));
        if (GetColor(brother) == kRed) {
          SetColor(brother, kBlack);
          SetColor(GetParent(node), kRed);
          RightRotate(GetParent(node));
          brother = GetLeft(GetParent(node));
        }
        if (GetColor(GetRight(brother)) == kBlack && GetColor(GetLeft(brother)) == kBlack) {
          SetColor(brother, kRed);
          node = GetParent(node);
        } else if (GetColor(GetLeft(brother)) == kBlack) {
          SetColor(GetRight(brother), kBlack);
          SetColor(brother, kRed);
          LeftRotate(brother);
          brother = GetLeft(GetParent(node));
        } else {
          SetColor(brother, GetColor(GetParent(node)));
          SetColor(GetParent(node), kBlack);
          SetColor(GetLeft(brother), kBlack);
          RightRotate(GetParent(node));
          while (GetParent(node) != nullptr) {
            node = GetParent(node);
          }
        }
      }
    }
    SetColor(node, kBlack);
    while (GetParent(root_) != nullptr) {
      root_ = GetParent(root_);
    }
  }

  static bool IsEqual(const ValueType &lhs, const ValueType &rhs) {
    return !(lhs < rhs || rhs < lhs);
  }

  inline Node *Find(const ValueType &elem) const {
    auto cur = root_;
    while (cur != nullptr && !IsEqual(elem, cur->GetValue())) {
      if (elem < cur->GetValue()) {
        cur = GetLeft(cur);
      } else {
        cur = GetRight(cur);
      }
    }
    return cur;
  }

  inline static Node *GetNext(Node *node) {
    if (GetRight(node)) {
      node = GetRight(node);
      while (GetLeft(node)) {
        node = GetLeft(node);
      }
      return node;
    }
    while (GetParent(node)) {
      if (GetLeft(GetParent(node)) == node) {
        return GetParent(node);
      }
      node = GetParent(node);
    }
    return nullptr;
  }

  inline static Node *GetPrev(Node *node) {
    if (GetLeft(node)) {
      node = GetLeft(node);
      while (GetRight(node)) {
        node = GetRight(node);
      }
      return node;
    }
    while (GetParent(node)) {
      if (GetRight(GetParent(node)) == node) {
        return GetParent(node);
      }
      node = GetParent(node);
    }
    return nullptr;
  }

public:
  class iterator {
  private:
    Node *node_ = nullptr;
    Node *root_ = nullptr;
    bool is_end_ = false;

    iterator(bool is_end, Node *root) : root_(root), is_end_(is_end) {}
    iterator(Node *node, Node *root) : node_(node), root_(root), is_end_(false) {}

    friend Set;
  public:
    iterator() = default;

    inline iterator(const iterator &other) = default;

    inline iterator &operator=(const iterator &other) = default;

    inline iterator operator++() {
      auto next = GetNext(node_);
      node_ = next;
      if (next == nullptr) {
        is_end_ = true;
      }
      return *this;
    }

    inline iterator operator++(int) {
      auto state = *this;
      auto next = GetNext(node_);
      node_ = next;
      if (next == nullptr) {
        is_end_ = true;
      }
      return state;
    }

    inline iterator operator--() {
      if (is_end_) {
        node_ = GetTreeMax(root_);
        is_end_ = false;
      } else {
        node_ = GetPrev(node_);
      }
      return *this;
    }

    inline iterator operator--(int) {
      auto state = *this;
      if (is_end_) {
        node_ = GetTreeMax(root_);
        is_end_ = false;
      } else {
        node_ = GetPrev(node_);
      }
      return state;
    }

    inline const ValueType *operator->() const {
      return &node_->GetValue();
    }

    inline const ValueType &operator*() const {
      return node_->GetValue();
    }

    inline bool operator==(const iterator &other) const {
      if (is_end_ && other.is_end_) {
        return other.root_ == root_;
      } else {
        return other.root_ == root_ && other.node_ == node_ && other.is_end_ == is_end_;
      }
    }

    inline bool operator!=(const iterator &other) const {
      return !(other == *this);
    }
  };

  inline iterator begin() const {
    if (empty()) {
      return {true, root_};
    }
    return {node_begin_, root_};
  }

  inline iterator end() const {
    if (empty()) {
      return {true, root_};
    }
    iterator it(node_end_, root_);
    ++it;
    return it;
  }

  inline iterator find(const ValueType &elem) const {
    if (empty()) {
      return end();
    }
    auto node = Find(elem);
    if (!node || !IsEqual(node->GetValue(), elem)) {
      return end();
    }
    return iterator(node, root_);
  }

  inline size_t size() const {
    return size_;
  }

  inline bool empty() const {
    return size_ == 0;
  }

  iterator lower_bound(const ValueType &elem) const {
    if (empty()) {
      return {true, root_};
    }
    Node *node = root_;
    Node *prev = nullptr;
    while (node && !IsEqual(node->GetValue(), elem)) {
      prev = node;
      if (elem < node->GetValue()) {
        node = GetLeft(node);
      } else {
        node = GetRight(node);
      }
    }
    if (node && IsEqual(node->GetValue(), elem)) {
      return {node, root_};
    } else if (prev->GetValue() < elem) {
      iterator it(prev, root_);
      ++it;
      return it;
    } else {
      return {prev, root_};
    }
  }

  inline bool contains(const ValueType &elem) const {
    if (empty()) {
      return false;
    }
    return find(elem) != end();
  }

  void insert(const ValueType &elem) {
    if (contains(elem)) {
      return;
    }
    ++size_;
    Insert(new Node(elem, kRed));
    node_begin_ = GetTreeMin(root_);
    node_end_ = GetTreeMax(root_);
  }

  void erase(const ValueType &elem) {
    auto node = Find(elem);
    if (node) {
      if (size_ == 1 && node == root_) {
        delete root_;
        root_ = node_begin_ = node_end_ = nullptr;
        --size_;
        return;
      }
      Erase(node);
      if (GetRight(GetParent(node)) == node) {
        SetRight(GetParent(node), nullptr);
      }
      if (GetLeft(GetParent(node)) == node) {
        SetLeft(GetParent(node), nullptr);
      }
      SetLeft(node, nullptr);
      SetRight(node, nullptr);
      delete node;
      node_begin_ = GetTreeMin(root_);
      node_end_ = GetTreeMax(root_);
      --size_;
    }
  }

  Set() : root_(nullptr), node_begin_(nullptr), node_end_(nullptr), size_(0) {}

  Set(std::initializer_list<ValueType> list) : Set() {
    for (const auto &el: list) {
      insert(el);
    }
  }

  template<typename AnotherIterator>
  Set(AnotherIterator start, AnotherIterator finish) : Set() {
    for (auto it = start; it != finish; ++it) {
      insert(*it);
    }
  }

  Set(const Set &other) : Set() {
    if (size_ != 0) {
      delete root_;
      root_ = nullptr;
    }
    if (other.size_ > 0) {
      root_ = CopyTree(other.root_);
      node_begin_ = GetTreeMin(root_);
      node_end_ = GetTreeMax(root_);
      size_ = other.size_;
    }
  }

  Set &operator=(const Set &other) {
    if (root_ == other.root_) {
      return *this;
    }
    if (size_ != 0) {
      delete root_;
      root_ = node_begin_ = node_end_ = nullptr;
      size_ = 0;
    }
    if (other.size_ > 0) {
      root_ = CopyTree(other.root_);
      node_begin_ = GetTreeMin(root_);
      node_end_ = GetTreeMax(root_);
      size_ = other.size_;
    }
    return *this;
  }

  ~Set() {
    if (size_ != 0) {
      delete root_;
    }
  }
};

#endif //RBTREE_H
