class Line {
 public:
  long long a, b;
  Line(int _a, int _b) { a = _a; b = _b; }
  Line() { a = 0; b = 0; }
  long long getY(int y) { return a * y + b; }
};
class Node {
 public:
  Line line;
  Node *leftNode, *rightNode;
  Node(Line _line) { line = _line; leftNode = NULL; rightNode = NULL; }
  Node() { leftNode = NULL; rightNode = NULL; }
};
class IntervalLineTree {
 private:
  int n;
  Node *root;

  void update(Node *node, int lo, int hi, Line &line) {
    int mid = (lo + hi) >> 1;
    if (line.getY(lo) <= line.getY(hi) && line.getY(hi) <= node->line.getY(hi))
      return;
    if (line.getY(lo) >= node->line.getY(lo) &&
        line.getY(hi) >= node->line.getY(hi)) {
      node->line = line;
      return;
    }
    if (node->leftNode == NULL) {
      node->leftNode = new Node();
    }
    if (node->rightNode == NULL) {
      node->rightNode = new Node();
    }
    if (line.getY(lo) <= node->line.getY(lo) &&
        line.getY(mid) <= node->line.getY(mid)) {
      update(node->rightNode, mid + 1, hi, line);
      return;
    }
    if (line.getY(lo) >= node->line.getY(lo) &&
        line.getY(mid) >= node->line.getY(mid)) {
      update(node->rightNode, mid + 1, hi, node->line);
      node->line = line;
      return;
    }
    if (line.getY(mid + 1) <= node->line.getY(mid + 1) &&
        line.getY(hi) <= node->line.getY(hi)) {
      update(node->leftNode, lo, mid, line);
    }

    if (line.getY(mid + 1) >= node->line.getY(mid + 1) &&
        line.getY(hi) >= node->line.getY(hi)) {
      update(node->leftNode, lo, mid, node->line);
      node->line = line;
    }
  }
  long long get(Node *node, int lo, int hi, int pos) {
    if (lo > pos || hi < pos) return 0;
    long long res = node->line.getY(pos);
    if (lo == hi) return res;
    int mid = (lo + hi) >> 1;
    if (node->leftNode != NULL)
      res = max(res, get(node->leftNode, lo, mid, pos));
    if (node->rightNode != NULL)
      res = max(res, get(node->rightNode, mid + 1, hi, pos));
    return res;
  }

 public:
  IntervalLineTree(int _n) { n = _n; root = new Node(); }
  void update(Line &line) { update(root, 1, n, line); }
  long long get(int pos) { return get(root, 1, n, pos); }
};
