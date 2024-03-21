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
	
}

/*
 * Returns the number of leaf nodes in the tree.
 *
 * You may want a recursive helper function for this.
 */
int TripleTree::NumLeaves() const {
    // replace the line below with your implementation
    return -1;
}

/**
     * Destroys all dynamically allocated memory associated with the
     * current TripleTree object. To be completed for PA3.
     * You may want a recursive helper function for this one.
     */
void TripleTree::Clear() {
    // add your implementation below
	
}

/**
 * Copies the parameter other TripleTree into the current TripleTree.
 * Does not free any memory. Called by copy constructor and operator=.
 * You may want a recursive helper function for this one.
 * @param other - The TripleTree to be copied.
 */
void TripleTree::Copy(const TripleTree& other) {
    // add your implementation below
	
}

/**
 * Private helper function for the constructor. Recursively builds
 * the tree according to the specification of the constructor.
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
    returnNode->avg = avgColor(im, ul, w, h);

    if ((w == 1) && (h == 1)){
        // If its exactly 1 pixel, don't need to make any children; just return
        return returnNode;
    }

    // Determine dimensions of child nodes:
    if (w >= h) {
        // "If the region to be divided is a square, then apply the Split wide behaviour" - tripletree.h
        unsigned int w_A = w / 3;
        unsigned int w_B = w_A;
        unsigned int w_C = w_A;

        if (w % 3 == 1){
            w_B++;
        } else if (w % 3 == 2){
            w_A++;
            w_C++; // <---- omg C++ language title drop !!1!1!
        }

        pair<unsigned int, unsigned int> ul_B(ul.first + w_A, ul.second);
        pair<unsigned int, unsigned int> ul_C(ul.first + w_A + w_B, ul.second);

        returnNode->A = BuildNode(im, ul, w_A, h);
        returnNode->B = BuildNode(im, ul_B, w_B, h);
        returnNode->C = BuildNode(im, ul_C, w_C, h);

    } else {
        unsigned int h_A = h / 3;
        unsigned int h_B = h_A;
        unsigned int h_C = h_A;

        if (h % 3 == 1){
            h_B++;
        } else if (h % 3 == 2){
            h_A++;
            h_C++;
        }

        pair<unsigned int, unsigned int> ul_B(ul.first, ul.second + h_A);
        pair<unsigned int, unsigned int> ul_C(ul.first, ul.second + h_A + h_C);

        returnNode->A = BuildNode(im, ul, w, h_A);
        returnNode->B = BuildNode(im, ul_B, w, h_B);
        returnNode->C = BuildNode(im, ul_C, w, h_C);
    }

    return returnNode;
}

/* ===== IF YOU HAVE DEFINED PRIVATE MEMBER FUNCTIONS IN tripletree_private.h, IMPLEMENT THEM HERE ====== */

/**
 * Return the average color of this section of the image.
 * Assumes that the entire rectanglular region starting at ul with height h and width w is in range for PNG im.
 * 
 * Also disregards the alpha channel - probably not needed?
*/
RGBAPixel TripleTree::avgColor(PNG& im, pair<unsigned int, unsigned int> ul, unsigned int w, unsigned int h){
    unsigned int totalRed = 0;
    unsigned int totalGreen = 0;
    unsigned int totalBlue = 0;
    unsigned int numPix = (w * h);
    RGBAPixel* currPix;

    for (unsigned int x = ul.first; x < (ul.first + w); x++){
        for (unsigned int y = ul.second; y < (ul.second + h); y++){
            currPix = im.getPixel(x, y);
            totalRed += currPix->r;
            totalGreen += currPix->g;
            totalBlue += currPix->b;
        }
    }

    return RGBAPixel(totalRed/numPix, totalGreen/numPix, totalBlue/numPix);
}

/**
 * recursive helper function for render()
*/
void TripleTree::renderRecursive(PNG& im, Node* node) const {
    
    if (node == nullptr){
        return;
    }

    if ((node->A == nullptr) && (node->B == nullptr) && (node->C == nullptr)){
        // If the node has 0 children, fill in pixels.

        for (unsigned int x = node->upperleft.first; x < node->upperleft.first + node->width; x++){
            for (unsigned int y = node->upperleft.second; y < node->upperleft.first + node->width; y++){
                *im.getPixel(x, y) = node->avg;
            }
        }
        
    } else {
        // otherwise the node has >=1 child -> recurse.
        // if >= 1 child, A and C are guaranteed, but need to check if B exists.

        renderRecursive(im, node->A);
        renderRecursive(im, node->C);

        if (node->B != nullptr){
            renderRecursive(im, node->B);
        }
    }
}