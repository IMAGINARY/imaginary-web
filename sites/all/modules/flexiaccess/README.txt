README.txt for Flexi Access
===========================

The Flexi Access module provides a simple interface to the ACL
(Access Control List) module. It lets you set up and mange ACLs naming
individual users that are allowed access to a particular node.

The idea behind Flexi Access is to allow per user acces control for a
node without the complexity or features of Organic Lists, and without
having to create a lot of (overlapping) roles.

Some of the code used in Flexi Access is lifted from the Content
Access module's user access control lists panel.  If this is all you
need, you may not need Flexi Access, so check that out first.

What Flexi Access has to offer (beyond the user access control lists
panel in Content Accees is a richer user interface that, among other
things, allows the administrator to create and maintain predefined
user lists.


Installation
------------

Installation should be below the directory here you keep your site's
contributed modules (this is usually sites/all/modules).


1. First, unless you've already done so, download, unpack and install
   ACL (http://drupal.org/project/acl).  Flexi Access depends on it
   being present.
2. Download, and unpack the install Flexi Access.
3. Enable Flexi Access from admin/modules.
4. You'll get a message telling you to rebuild permissions. Do it!
5. Flexi Access is now ready for use.


Usage
-----

The administrative interface (admin/config/people/flexiaccess) has the
following tab:

- Content types: Enable/disable the content types you want
  Flexi Access to manage (e.g. Article).

Administrators viewing a node of an Flexi Access enabled content type
will see a tab called 'Flexi Access'.  If you click this for a node
that has no ACL (Access Control List) associated, you'll see a button
to create an ACL.  Until you create an ACL, access will not be managed
by Flexi Access.  When you create an ACL, access will be managed by
Flexi Access and only the users named in the node's ACL will be able
to see the node.

When Flexi Access manages a node, there will be three subfields (view,
update, delete) inside the Flexi Access tab. You can use these
subfields to manage individual users' view, update, and delete
permissions for the node.


Running multiple node access modules on a site
----------------------------------------------

If you're using the Flexi Access as well as the Content Access module,
and enable the per node access control settings for a content type,
you will have to almost identical interfaces for per user access
control.  It is not recommended to use both in parallell.

Also note that a Drupal node access module should only grant access to
content nodes, not deny it. So if you are using well-behaved multiple
node access modules, access will be granted as soon as at least one of
the modules grants it.

According to the documentation, you can influence the behaviour when
running multiple access modules by changing the priority of a content
access module.  Only modules with the same priority is supposed to
accumulate access as described in the preceding paragraph.  If
priorites differm the module with the highest priority shall take
precedence.  For example,  if the module has the highest priority
alone, it alone will determine access.  However, I've not managed
to get this to work.


Whishlist
---------

1. Integration with Rules module:
   http://drupal.org/node/1879112

2. What is needed is an UI to create a user group to contain
   predefined list of users.  E.g. a user could be from the role
   student, but could be assigned to group classA, paperB, yearC,
   etc. to access different content.

   Then, on the node a UI to attach an initial ACL consisting of one
   of these lists.  It should be possible to add a group, or to
   replace the ACL with a different group.

3. JavsScript to warn user if moving away from page without commiting.

I welcome patches from anyone that provides one of the functions
listed above.


Bug reports, feature requests, patches
--------------------------------------

Post any bug reports, feature requests, support questions and patches
to the issue queue:

   http://drupal.org/project/issues/flexiaccess


Related
-------

- http://drupal.org/node/1064762#comment-6427868
- Simple Access: http://drupal.org/node/1397074
