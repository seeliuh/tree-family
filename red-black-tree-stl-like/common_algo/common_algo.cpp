#include "../bst_tree.hh"
#include <functional>
#include <stack>
#include "../unit_test/rb_tree_debug_tool.hh"
#include "../unit_test/test_data.hh"

void pre_order_pass(bst_tree<test_data > *br) {
    printf("pre_order_pass:");
    bst_tree<test_data >::bstnode_t *node = br->head;
    std::stack<bst_tree<test_data >::bstnode_t * > sk;
    if(node != br->end_flag)
        sk.push(node);
    while(!sk.empty()) {
        node = sk.top();
        sk.pop();
        printf("%d ", node->data.index);
        if(node->right != br->end_flag)
            sk.push(node->right);
        if(node->left != br->end_flag)
            sk.push(node->left);
    }
    printf("\n");
}

void in_order_pass(bst_tree<test_data > *br) {
    printf("in_order_pass:");
    bst_tree<test_data >::bstnode_t *node = br->head;
    std::stack<bst_tree<test_data >::bstnode_t * > sk;
    while(node!=br->end_flag || !sk.empty()) {
        while(node!= br->end_flag) {
            sk.push(node);
            node = node->left;
        }
        if(!sk.empty()){
             node = sk.top();
             printf("%d ", node->data.index);
             sk.pop();
             node = node->right;

        }
    }
    printf("\n");
}

void post_order_pass(bst_tree<test_data > *br) {
    printf("post_order_pass:");
    bst_tree<test_data >::bstnode_t *node = br->head->left, *has_visited = 0;
    std::stack<bst_tree<test_data >::bstnode_t * > sk;
    sk.push(br->head);
    while(!sk.empty()) {
        if(has_visited!= node) {
            while(node!=br->end_flag) {
                sk.push(node);
                node = node->left;
            }
        }
        if(!sk.empty()) {
            node = sk.top();
            if(has_visited == node->right || node->right == br->end_flag) {
                has_visited = node;
                printf("%d ", node->data.index);
                sk.pop();
            }else {
                node = node->right;
            }
        }
    }
    printf("\n");
}

typedef struct BITMAPFILEHEADER
{
    short bfType;
    int bfSize;
    short bfReserved1;
    short bfReserved2;
    int bfOffBits;
}BITMAPFILEHEADER; //14bit

typedef struct BITMAPINFOHEADER
{
    int biSize;
    int biWidth;
    int biHeight;
    short biPlanes;
    short biBitCount;
    int biCompression;
    int biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    int biClrUsed;
    int biClrImportant;
}BITMAPINFODEADER;//40bit

bool SaveRGBImg(const char* lpszFileName, void* pImgData, int nWidth, int nHeight)
{
	int nStride = (nWidth*3+3)/4*4;
	int imageSize = abs(nStride*nHeight);
	BITMAPFILEHEADER bmf;
	BITMAPINFOHEADER bmi;
	memset(&bmf, 0, sizeof(bmf));
	memset(&bmi, 0, sizeof(bmi));

	// file header
	bmf.bfType = 0x4d42;
	bmf.bfOffBits = sizeof(bmf) + sizeof(bmi);
	bmf.bfSize = imageSize + bmf.bfOffBits;

	// info header
	bmi.biSize = sizeof(bmi);
	bmi.biWidth = nWidth;
	bmi.biHeight = nHeight;
	bmi.biPlanes = 1;
	bmi.biBitCount = 24;
	bmi.biCompression = 0;
	bmi.biSizeImage = imageSize;

	FILE *fp;
	fp = fopen(lpszFileName, "wb");
	if(fp != NULL)
	{
		fwrite(&bmf, sizeof(bmf), 1, fp);
		fwrite(&bmi, sizeof(bmi), 1, fp);
		fwrite(pImgData, 1, imageSize, fp);
		fclose(fp);
		return true;
	}

	return false;
}

int main() {
    rb_tree<test_data > tr;
    std::vector<test_data > vecElement;
    rbtree_tool::gen_test_tree(tr, 100, vecElement);
    getchar();
    rbtree_tool::print_rbtree(&tr);
    bst_tree<test_data >* rb = (bst_tree<test_data >*)&tr;
    pre_order_pass(rb);
    in_order_pass(rb);
    post_order_pass(rb);
}
