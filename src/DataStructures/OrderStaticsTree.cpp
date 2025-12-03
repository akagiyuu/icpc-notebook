#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;

// use pair for multiset
// st.insert(val)
// st.erase(val)
// st.find_by_order(k): return kth element
// st.order_of_key(val): count element in tree < val
template<class T> using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
