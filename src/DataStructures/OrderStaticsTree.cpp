#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;

template<class T> using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

//multiset thi dung pair
/*
st.insert(val):Thêm phần tử val vào cây.
st.erase(val):Xóa phần tử val khỏi cây.
st.find_by_order(k):Tìm phần tử ở vị trí thứ k (bắt đầu từ 0).Trả về một iterator.
st.order_of_key(val):Đếm số lượng phần tử trong cây nhỏ hơn val.Trả về một số nguyên (size_t).
*/