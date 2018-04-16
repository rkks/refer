=========================
Introduction to this book
=========================

This project started out when a group of people from Pelagicore went to FOSDEM
2016. We saw several very interesting talks on containers and talked among
ourselves about it and realized that we actually have quite a lot of experience
with the subject among ourselves. We decided to try and contribute into this book
and write something that may be of use to the community.

Relatively recent development in the Linux community has made containers a more
and more viable option as a full virtualization replacement and the number of
use cases has grown widely. This book will go on to explore and describe what
containers really are, the general concepts and techniques used in detail and
what tools are actually using these underlying technologies to do their
different jobs. We will also discuss the security implications of using
namespaces as compared to other security techniques traditionally employed to
either make a secure system or where it is a secondary benefit.

The book will try to be as pedagogical as possible basing a lot of the
learning on examples interwoven with theoretical information. Some of the
technical underpinnings such as the kernel APIs are constantly under
development so there may be issues running everything. Whenever we are aware of
an API that is unstable or may be up for revision we will try to indicate this,
alternatively if possible we will show how to perform a given task before and
after an API change.
