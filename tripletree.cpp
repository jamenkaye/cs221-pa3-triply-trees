/**
 * @file        tripletree.cpp
 * @description Student-implemented functions of a ternary tree for CPSC 221 PA3.
 *
 *              THIS FILE WILL BE SUBMITTED FOR GRADING
 */

#include "tripletree.h"

 /**
      * Constructor that builds a TripleTree out of the given PNG.
      *
      * The TripleTree represents the subimage from (0,0) to (w-1, h-1) where
      * w-1 and h-1 are the largest valid image coordinates of the original PNG.
      * Every node corresponds to a rectangle of pixels in the original PNG,
      * represented by an (x,y) pair for the upper left corner of the
      * square and two integers for the number of pixels on the width and
      * height dimensions of the rectangular region the node defines.
      *
      * The node's three children correspond to a partition
      * of the node's rectangular region into three approximately equal-size strips.
      *
      * If the rectangular region is taller than it is wide, the region is divided
      * into horizontal strips:
      *  +-------+
      *  |   A   |
      *  |       |
      *  +-------+
      *  |   B   |
      *  |       |    (Split tall)
      *  +-------+
      *  |   C   |
      *  |       |
      *  +-------+
      *
      * If the rectangular region is wider than it is tall, the region is divided
      * into vertical strips:
      *  +---------+---------+---------+
      *  |    A    |    B    |    C    |
      *  |         |         |         |    (Split wide)
      *  +---------+---------+---------+
      *
      * Your regions are not guaranteed to have dimensions exactly divisible by 3.
      * If the dimensions of your rectangular region are 3p x q or q x 3p where 3p
      * represents the length of the long side, then your rectangular regions will
      * each have dimensions p x q (or q x p)
      *
      * If the dimensions are (3p+1) x q, subregion B gets the extra pixel of size
      * while subregions A and C have dimensions p x q.
      *
      * If the dimensions are (3p+2) x q, subregions A and C each get an extra pixel
      * of size, while subregion B has dimensions p x q.
      *
      * If the region to be divided is a square, then apply the Split wide behaviour.
      *
      * Every leaf in the constructed tree corresponds to a pixel in the PNG.
      *
      * @param imIn - the input image used to construct the tree
      */
TripleTree::TripleTree(PNG& imIn) {
    // add your implementation below

    pair<unsigned int, unsigned int> ul(0, 0);
    this->root = BuildNode(imIn, ul, imIn.width(), imIn.height());
	
}

/**
 * Render returns a PNG image consisting of the pixels
 * stored in the tree. It may be used on pruned trees. Draws
 * every leaf node's rectangle onto a PNG canvas using the
 * average color stored in the node.
 *
 * You may want a recursive helper function for this.
 */
PNG TripleTree::Render() const {
    // replace the line below with your implementation
    // return PNG();

    PNG im(this->root->width, this->root->height);
    renderRecursive(im, this->root);
    return im;
}

/*
 * Prune function trims subtrees as high as possible in the tree.
 * A subtree is pruned (cleared) if all of its leaves are within
 * tol of the average color stored in the root of the subtree.
 * Pruning criteria should be evaluated on the original tree, not
 * on a pruned subtree. (we only expect that trees would be pruned once.)
 *
 * You may want a recursive helper function for this.
 *
 * @param tol - maximum allowable RGBA color distance to qualify for pruning
 */
void TripleTree::Prune(double tol) {
    // add your implementation below

    recursivePrune(this->root, this->root->avg, tol);
}

/**
 * Rearranges the tree contents so that when rendered, the image appears
 * to be mirrored horizontally (flipped over a vertical axis).
 * This may be called on pruned trees and/or previously flipped/rotated trees.
 *
 * You may want a recursive helper function for this.
 */
void TripleTree::FlipHorizontal() {
    // add your implementation below

    //start at the root and recursively call on the tree
    recursiveFlipHorz(root);

    return;
}

/**
 * Rearranges the tree contents so that when rendered, the image appears
 * to be rotated 90 degrees counter-clockwise.
 * This may be called on pruned trees and/or previously flipped/rotated trees.
 *
 * You may want a recursive helper function for this.
 */
void TripleTree::RotateCCW() {
    // add your implementation below

    // Need to swap root's width and height.
    swapHeightWidth(root);
    // root->upperleft will still be (0, 0).

    recursiveRotateCCW(root);

    return;
}

/*
 * Returns the number of leaf nodes in the tree.
 *
 * You may want a recursive helper function for this.
 */
int TripleTree::NumLeaves() const {
    // replace the line below with your implementation

    return recursiveNumLeaves(root);
}

/**
     * Destroys all dynamically allocated memory associated with the
     * current TripleTree object. To be completed for PA3.
     * You may want a recursive helper function for this one.
     */
void TripleTree::Clear() {
    // add your implementation below
	Clear(root); //start from the root
    root = NULL; 
}

/**
 * Copies the parameter other TripleTree into the current TripleTree.
 * Does not free any memory. Called by copy constructor and operator=.
 * You may want a recursive helper function for this one.
 * @param other - The TripleTree to be copied.
 */
void TripleTree::Copy(const TripleTree& other) {
    // add your implementation below
	if (this == &other) { //check if the tree is the same prior to copying
        return;
    }

    Clear();

    root = copyNode(other.root);
}

/**
 * Private helper function for the constructor. Recursively builds
 * the tree according to the specification of the constructor.
 * 
 * Note: Can't do iterative color averaging: https://piazza.com/class/lr2vkjsws5p7bb/post/1722
 * 
 * @param im - reference image used for construction
 * @param ul - upper left point of node to be built's rectangle.
 * @param w - width of node to be built's rectangle.
 * @param h - height of node to be built's rectangle.
 */
Node* TripleTree::BuildNode(PNG& im, pair<unsigned int, unsigned int> ul, unsigned int w, unsigned int h) {
    // replace the line below with your implementation
    // return nullptr;

    if ((w == 0) || (h == 0)){
        // If either dimension is 0, there are no pixels.
        return nullptr;
    }

    Node* returnNode = new Node(ul, w, h); // Child nodes are initialized to null by default

    if ((w == 1) && (h == 1)){
        // If its exactly 1 pixel, don't need to make any children

        returnNode->avg = *im.getPixel(ul.first, ul.second);
        return returnNode;
    }

    // Determine dimensions of child nodes:
    int longDimension = std::max(w, h);

    int dividedA = longDimension / 3;
    int dividedB = dividedA;
    int dividedC = dividedA;

    if (longDimension % 3 == 1){
        dividedB++;
    } else if (longDimension % 3 == 2){
        dividedA++;
        dividedC++; // <---- omg C++ language title drop !!1!1!
    }

    if (w >= h) {
        // "If the region to be divided is a square, then apply the Split wide behaviour" - tripletree.h
        // Thats why this case includes the equality

        pair<unsigned int, unsigned int> ul_B(ul.first + dividedA, ul.second);
        pair<unsigned int, unsigned int> ul_C(ul.first + dividedA + dividedB, ul.second);
        
        returnNode->A = BuildNode(im, ul, dividedA, h);
        returnNode->B = BuildNode(im, ul_B, dividedB, h);
        returnNode->C = BuildNode(im, ul_C, dividedC, h);

        returnNode->avg = avgColor(returnNode, dividedA, dividedB);

    } else {
        pair<unsigned int, unsigned int> ul_B(ul.first, ul.second + dividedA);
        pair<unsigned int, unsigned int> ul_C(ul.first, ul.second + dividedA + dividedB);

        returnNode->A = BuildNode(im, ul, w, dividedA);
        returnNode->B = BuildNode(im, ul_B, w, dividedB);
        returnNode->C = BuildNode(im, ul_C, w, dividedC);

        returnNode->avg = avgColor(returnNode, dividedA, dividedB);
    }

    return returnNode;
}

/* ===== IF YOU HAVE DEFINED PRIVATE MEMBER FUNCTIONS IN tripletree_private.h, IMPLEMENT THEM HERE ====== */

/**
 * Return the WEIGHTED average color of the children nodes of this node
 * 
 * Assumes that the node has at least 2 children.
*/
RGBAPixel TripleTree::avgColor(Node* node, int dividedA, int dividedB){
    int longDimension = std::max(node->width, node->height);

    int totalR = (node->A->avg.r + node->C->avg.r) * dividedA;
    int totalG = (node->A->avg.g + node->C->avg.g) * dividedA;
    int totalB = (node->A->avg.b + node->C->avg.b) * dividedA;
    int totalA = (node->A->avg.a + node->C->avg.a) * dividedA;

    if (node->B != NULL){
        totalR += node->B->avg.r * dividedB;
        totalG += node->B->avg.g * dividedB;
        totalB += node->B->avg.b * dividedB;
        totalA += node->B->avg.a * dividedB;
    }

    return RGBAPixel(totalR/longDimension, totalG/longDimension, totalB/longDimension, totalA/longDimension);
}

/**
 * recursive helper function for render()
*/
void TripleTree::renderRecursive(PNG& im, Node* node) const {

    if (node == nullptr){
        return;
    }

    if (node->A == nullptr){
        // If A is null -> node has 0 children -> fill in pixels.

        for (unsigned int x = node->upperleft.first; x < node->upperleft.first + node->width; x++){
            for (unsigned int y = node->upperleft.second; y < node->upperleft.second + node->height; y++){
                *im.getPixel(x, y) = node->avg;
            }
        }
        
    } else {
        // otherwise the node has >=1 child -> recurse.
        // if >= 1 child, A and C are guaranteed, but need to check if B exists.

        renderRecursive(im, node->A);
        if (node->B != nullptr){
            renderRecursive(im, node->B);
        }
        renderRecursive(im, node->C);
    }
}

/**
 * recursive helper function for clear()
*/
void TripleTree::Clear(Node*& node) {
    //null base case
    if (node == NULL) {
        return;
    }

    Clear(node->A); //clear left/upper
    Clear(node->B); //clear middle
    Clear(node->C); //clear right/lower

    delete node;
    node = NULL;
}

/**
 * recursive helper function for copy()
*/
Node* TripleTree::copyNode(Node* other) {
    if (other == NULL) { //if the tree is already the same
        return NULL;
    }

    //create a new node and copy data into it
    Node* newNode = new Node(other->upperleft, other->width, other->height);
    newNode->avg = other->avg;

    //copy the children recursively
    newNode->A = copyNode(other->A);
    newNode->B = copyNode(other->B);
    newNode->C = copyNode(other->C);

    return newNode;
}

/**
 * Helper function for prune()
 * Finds the maximum distance betwee the given color and one of the children of node
*/
double TripleTree::maxDistToChildColor(Node* node, RGBAPixel& color) const {
    
    if (node == NULL){
        return 0.0;
    }

    if (node->A == NULL){
        // If the node is a leaf, calculate the distance and return it. Node must be a leaf is child A is null.
        return (node->avg).distanceTo(color);
    }

    // If the node is not a leaf, return the max dist of any of the children
    return std::max(maxDistToChildColor(node->A, color), 
                    std::max(maxDistToChildColor(node->B, color), maxDistToChildColor(node->C, color)));
}

/**
 * Recursive helper function for prune that does the pruning
*/
void TripleTree::recursivePrune(Node* node, RGBAPixel& color, double tol){
    if (node == NULL){
        return;
    }

    if (TripleTree::maxDistToChildColor(node, color) <= tol){
        // Delete children
        delete node->A;
        node->A = NULL;
        delete node->C;
        node->C = NULL;

        if (node->B != NULL){
            delete node->B;
            node->B = NULL;
        }

    } else {
        // Check children
        recursivePrune(node->A, color, tol);
        recursivePrune(node->B, color, tol);
        recursivePrune(node->C, color, tol);
    }
}

/**
 * Recursive helper function for NumLeaves, returns numleaves
*/
int TripleTree::recursiveNumLeaves(Node* node) const {
    if (root == nullptr){
        return 0;
    }

    Node* rootNode = node;
    int numLeaves = 0;

    //goes down every branch
    if (node->A != NULL) {
        recursiveNumLeaves(node->A);
    }
    if (node->B != NULL) {
        recursiveNumLeaves(node->B);
    }
    if (node->C != NULL) {
        recursiveNumLeaves(node->C);
    }

    numLeaves++;
    return numLeaves;
}

/**
 * Recursie helper function for rotateCCW
*/
void TripleTree::recursiveRotateCCW(Node* node){

    if (node == NULL){
        return;
    }

    if (node->A == NULL){
        // If A is null, node must not have any children.
        // This node's UL, height, and width were all updated when it's parent node was "node".
        return;
    }
    // Now, children A and C are guaranteed. B may still be null.

    int b_min_dim = 0;

    // Swap children dimensions:
    swapHeightWidth(node->A);
    if (node->B != NULL){
        swapHeightWidth(node->B);
        b_min_dim = std::min(node->B->height, node->B->width);
    }
    swapHeightWidth(node->C);


    // if (node->C->upperleft.second == node->upperleft.second){
        // Same y-vals before updating -> need to do a landscape to portrait rotation

    if ((2*node->A->height + b_min_dim) == node->height){
        // If the new heights add up, the old widths added up -> need to do a landscape to portrait rotation
        
        // Swap A and C
        Node* temp = node->C;
        node->C = node->A;
        node->A = temp;

        // Update children's UL corners
        node->A->upperleft = node->upperleft;
        if (node->B != NULL){
            node->B->upperleft.first = node->upperleft.first;
            node->B->upperleft.second = node->upperleft.second + node->A->height;
        }
        node->C->upperleft.first = node->upperleft.first;
        node->C->upperleft.second = node->upperleft.second + node->A->height + b_min_dim;

    } else {
        // need to do a portrait to landscape rotation. DON'T swap A and C

        // Update children's UL corners
        node->A->upperleft = node->upperleft;
        if (node->B != NULL){
            node->B->upperleft.first = node->upperleft.first + node->A->width;
            node->B->upperleft.second = node->upperleft.second;
        }
        node->C->upperleft.first = node->upperleft.first + node->A->width + b_min_dim;
        node->C->upperleft.second = node->upperleft.second;
    }

    // Recurse to children
    recursiveRotateCCW(node->A);
    recursiveRotateCCW(node->B);
    recursiveRotateCCW(node->C);
}

/**
 * Helper function for rotate
 * Swaps the values in the height and width fields of node
*/
void TripleTree::swapHeightWidth(Node* node){
    unsigned int temp = node->height;
    node->height = node->width;
    node->width = temp;
}

/**
 * Helper function for flip horizontal
*/
void TripleTree::recursiveFlipHorz(Node* node) {
    if (node == nullptr) {
        return;
    }

    if (node->width >= node->height) {
        //swap node A and B
        Node* tempNode = node->A;
        node->A = node->C;
        node->C = tempNode;


        if (node->A != nullptr && node->C != nullptr) {
            // Update children's UL corners
            node->A->upperleft = node->upperleft;

            if (node->B != nullptr){
                node->C->upperleft.first = node->upperleft.first + node->A->width + node->B->width;
            } else 
                node->C->upperleft.first = node->upperleft.first + node->A->width;
        }
    } else {
        if (node->A != nullptr && node->C != nullptr) {
            // Update children's UL corners
            node->A->upperleft.first = node->upperleft.first;
            node->C->upperleft.first = node->upperleft.first;

            if (node->B != nullptr) {
                node->B->upperleft.first = node->upperleft.first;
            }
        }
    }

    recursiveFlipHorz(node->A);
    recursiveFlipHorz(node->B);
    recursiveFlipHorz(node->C);
}