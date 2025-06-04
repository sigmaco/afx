/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                 Public Test Build
 *                             (c) 2017 SIGMA FEDERATION
 *                      Open sourced under the Qwadro License.
 */

#include "qwadro/inc/afxQwadro.h"

  //////////////////////////////////////////////////////////////////////////////
 // TEST SUITE | afxChain                                                    //
//////////////////////////////////////////////////////////////////////////////

// Test object embedding afxLink
typedef struct MyNode {
    int value;
    afxLink link;
} MyNode;

_AFX void test_deploy_and_basic_ops(void)
{
    afxError err = NIL;
    afxChain chain;
    AfxDeployChain(&chain, NULL);
    AFX_ASSERT(AfxIsChainEmpty(&chain));
    AFX_ASSERT(AfxGetChainLength(&chain) == 0);
    AFX_ASSERT(AfxGetChainHolder(&chain) == NULL);
    AFX_ASSERT(AfxGetAnchor(&chain) == &chain.anchor);
}

void test_push_pop_links()
{
    afxError err = NIL;
    afxChain chain;
    AfxDeployChain(&chain, NULL);

    MyNode a = { .value = 1 };
    MyNode b = { .value = 2 };
    MyNode c = { .value = 3 };

    AFX_ASSERT(AfxPushLink(&b.link, &chain) == 1);
    AFX_ASSERT(AfxPushLink(&a.link, &chain) == 1); // a before b
    AFX_ASSERT(AfxPushBackLink(&c.link, &chain) == 1); // c at end

    AFX_ASSERT(AfxGetChainLength(&chain) == 3);

    afxLink* f = AfxGetFirstLink(&chain);
    afxLink* l = AfxGetLastLink(&chain);
    AFX_ASSERT(AFX_REBASE(f, MyNode, link)->value == 1);
    AFX_ASSERT(AFX_REBASE(l, MyNode, link)->value == 3);

    AFX_ASSERT(AfxGetNextLink(&a.link) == &b.link);
    AFX_ASSERT(AfxGetPrevLink(&b.link) == &a.link);
    AFX_ASSERT(AfxGetLinker(&b.link) == NULL); // chain->holder is NULL

    AFX_ASSERT(AfxGetChain(&a.link) == &chain);
    AFX_ASSERT(AfxGetChain(&b.link) == &chain);

    AFX_ASSERT(AfxFindLinkIndex(&chain, &b.link, NULL));
    AFX_ASSERT(AfxFindLinkIndexB2F(&chain, &a.link, NULL));
    AFX_ASSERT(AfxFindFirstLink(&chain, 0) == &a.link);
    AFX_ASSERT(AfxFindLastLink(&chain, 0) == &c.link);

    AfxPopLink(&b.link);
    AFX_ASSERT(AfxGetChainLength(&chain) == 2);
    AFX_ASSERT(b.link.chain == NULL);
}

void test_iteration_macros()
{
    afxError err = NIL;
    afxChain chain;
    AfxDeployChain(&chain, NULL);

    MyNode nodes[5];
    for (int i = 0; i < 5; ++i) {
        nodes[i].value = i * 10;
        AfxPushBackLink(&nodes[i].link, &chain);
    }

    int i = 0;
    MyNode* iter;
    AFX_ITERATE_CHAIN(MyNode, iter, link, &chain)
    {
        AFX_ASSERT(iter->value == i * 10);
        i++;
    }
    AFX_ASSERT(i == 5);

    int j = 4;
    AFX_ITERATE_CHAIN_B2F(MyNode, iter, link, &chain)
    {
        AFX_ASSERT(iter->value == j * 10);
        j--;
    }
    AFX_ASSERT(j == -1);
}

void test_linking_specific_positions()
{
    afxError err = NIL;
    afxChain chain;
    AfxDeployChain(&chain, NULL);

    MyNode a = { .value = 1 };
    MyNode b = { .value = 2 };
    MyNode c = { .value = 3 };

    AfxPushLink(&a.link, &chain);
    AfxLinkBehind(&b.link, &a.link);
    AfxLinkAhead(&c.link, &b.link); // order: a, c, b

    afxLink* first = AfxGetFirstLink(&chain);
    AFX_ASSERT(AFX_REBASE(first, MyNode, link)->value == 1);

    afxLink* mid = AfxGetNextLink(&a.link);
    AFX_ASSERT(AFX_REBASE(mid, MyNode, link)->value == 3);

    afxLink* last = AfxGetLastLink(&chain);
    AFX_ASSERT(AFX_REBASE(last, MyNode, link)->value == 2);
}

void test_swap_chains()
{
    afxError err = NIL;
    afxChain ch1, ch2;
    AfxDeployChain(&ch1, NULL);
    AfxDeployChain(&ch2, NULL);

    MyNode a = { .value = 1 };
    MyNode b = { .value = 2 };

    AfxPushBackLink(&a.link, &ch1);
    AfxPushBackLink(&b.link, &ch2);

    AfxSwapChains(&ch1, &ch2);

    AFX_ASSERT(AfxGetChainLength(&ch1) == 1);
    AFX_ASSERT(AFX_REBASE(AfxGetFirstLink(&ch1), MyNode, link)->value == 2);
    AFX_ASSERT(AFX_REBASE(AfxGetFirstLink(&ch2), MyNode, link)->value == 1);
}

void test_reset_link()
{
    afxError err = NIL;
    MyNode x = { .value = 99 };
    afxChain ch;
    AfxDeployChain(&ch, NULL);
    AfxPushBackLink(&x.link, &ch);

    AFX_ASSERT(x.link.chain == &ch);
    AfxResetLink(&x.link);
    AFX_ASSERT(x.link.chain == NULL);
    AFX_ASSERT(x.link.next == NULL);
    AFX_ASSERT(x.link.prev == NULL);
}

void test_append_prepend_chain()
{
    /*
        Full validation for AfxAppendChain and AfxPrependChain.
        Tests that validate order, lengths, and structural integrity.
        Guarantees that other is emptied and ch is updated.
    */
    
    afxError err;
    afxChain a, b;
    AfxDeployChain(&a, NULL);
    AfxDeployChain(&b, NULL);

    MyNode n1 = { .value = 1 };
    MyNode n2 = { .value = 2 };
    MyNode n3 = { .value = 3 };
    MyNode n4 = { .value = 4 };

    AfxPushBackLink(&n1.link, &a);
    AfxPushBackLink(&n2.link, &a);

    AfxPushBackLink(&n3.link, &b);
    AfxPushBackLink(&n4.link, &b);

    // Append b to a: a = [1,2,3,4], b should be empty
    AfxAppendChain(&a, &b);
    AFX_ASSERT(AfxGetChainLength(&a) == 4);
    AFX_ASSERT(AfxGetChainLength(&b) == 0);
    AFX_ASSERT(AfxIsChainEmpty(&b));

    int expected[] = {1, 2, 3, 4};
    int idx = 0;
    MyNode* iter;
    AFX_ITERATE_CHAIN(MyNode, iter, link, &a) {
        AFX_ASSERT(iter->value == expected[idx]);
        idx++;
    }

    // Refill b and prepend into a
    AfxPushBackLink(&n3.link, &b);
    AfxPushBackLink(&n4.link, &b);
    AfxPrependChain(&a, &b); // a = [3,4,1,2,3,4], b = empty

    int expected2[] = {3, 4, 1, 2, 3, 4};
    idx = 0;
    AFX_ITERATE_CHAIN(MyNode, iter, link, &a) {
        AFX_ASSERT(iter->value == expected2[idx]);
        idx++;
    }

    AFX_ASSERT(AfxGetChainLength(&b) == 0);
    AFX_ASSERT(AfxIsChainEmpty(&b));
}

int main()
{
    test_deploy_and_basic_ops();
    test_push_pop_links();
    test_iteration_macros();
    test_linking_specific_positions();
    test_swap_chains();
    test_reset_link();
    test_append_prepend_chain();
    AfxPrintf("All afxChain unit tests passed.\n");
    return 0;
}
