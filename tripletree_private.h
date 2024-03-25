/**
 * @file        tripletree_private.h
 * @description student declarations of private functions
 *              for TripleTree, CPSC 221 PA3
 *
 *              THIS FILE WILL BE SUBMITTED.
 *
 *              Simply declare your function prototypes here.
 *              No other scaffolding is necessary.
 */

 // begin your declarations below

RGBAPixel avgColor(PNG& im, pair<unsigned int, unsigned int> ul, unsigned int w, unsigned int h);

void renderRecursive(PNG& im, Node* node) const;

void Clear(Node*& node);