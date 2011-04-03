/*
 * Copyright (c) 2009 Mark Heily <mark@heily.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "common.h"

static int __thread kqfd;
static int __thread vnode_fd;
static char __thread testfile[1024];


/* Create an empty file */
static void
testfile_create(void)
{
    int fd;

    if ((fd = open(testfile, O_CREAT | O_WRONLY, 0600)) < 0)
        die("open");
    close(fd);
}

static void
testfile_touch(void)
{
    char buf[1024];

    snprintf(&buf[0], sizeof(buf), "touch %s", testfile);
    if (system(buf) != 0)
        die("system"); 
}

static void
testfile_write(void)
{
    char buf[1024];

    snprintf(&buf[0], sizeof(buf), "echo hi >> %s", testfile);
    if (system(buf) != 0)
        die("system"); 
}

static void
testfile_rename(int step)
{
    char buf[1024];

    snprintf(&buf[0], sizeof(buf), "%s.tmp", testfile);
    /* XXX-FIXME use of 'step' conceals a major memory corruption
            when the file is renamed twice.
            To replicate, remove "if step" conditional so
            two renames occur in this function.
            */
    if (step == 0) {
        if (rename(testfile,buf) != 0)
            err(1,"rename"); 
    } else {
        if (rename(buf, testfile) != 0)
            err(1,"rename"); 
    }
}

void
test_kevent_vnode_add(void)
{
    struct kevent kev;

    testfile_create();

    vnode_fd = open(testfile, O_RDWR);
    if (vnode_fd < 0)
        err(1, "open of %s", testfile);

    kevent_add(kqfd, &kev, vnode_fd, EVFILT_VNODE, EV_ADD, 
            NOTE_WRITE | NOTE_ATTRIB | NOTE_RENAME | NOTE_DELETE, 0, NULL);
}

void
test_kevent_vnode_note_delete(void)
{
    struct kevent kev;

    kevent_add(kqfd, &kev, vnode_fd, EVFILT_VNODE, EV_ADD | EV_ONESHOT, NOTE_DELETE, 0, NULL);

    if (unlink(testfile) < 0)
        die("unlink");

    kevent_cmp(&kev, kevent_get(kqfd));
}

void
test_kevent_vnode_note_write(void)
{
    struct kevent kev;

    kevent_add(kqfd, &kev, vnode_fd, EVFILT_VNODE, EV_ADD | EV_ONESHOT, NOTE_WRITE, 0, NULL);

    testfile_write();

    /* BSD kqueue adds NOTE_EXTEND even though it was not requested */
    /* BSD kqueue removes EV_ENABLE */
    kev.flags &= ~EV_ENABLE; // XXX-FIXME compatibility issue
    kev.fflags |= NOTE_EXTEND; // XXX-FIXME compatibility issue
    kevent_cmp(&kev, kevent_get(kqfd));
}

void
test_kevent_vnode_note_attrib(void)
{
    struct kevent kev;
    int nfds;

    kevent_add(kqfd, &kev, vnode_fd, EVFILT_VNODE, EV_ADD | EV_ONESHOT, NOTE_ATTRIB, 0, NULL);

    testfile_touch();

    nfds = kevent(kqfd, NULL, 0, &kev, 1, NULL);
    if (nfds < 1)
        die("kevent");
    if (kev.ident != vnode_fd ||
            kev.filter != EVFILT_VNODE || 
            kev.fflags != NOTE_ATTRIB)
        err(1, "%s - incorrect event (sig=%u; filt=%d; flags=%d)", 
                test_id, (unsigned int)kev.ident, kev.filter, kev.flags);
}

void
test_kevent_vnode_note_rename(void)
{
    struct kevent kev;
    int nfds;

    kevent_add(kqfd, &kev, vnode_fd, EVFILT_VNODE, EV_ADD | EV_ONESHOT, NOTE_RENAME, 0, NULL);

    testfile_rename(0);

    nfds = kevent(kqfd, NULL, 0, &kev, 1, NULL);
    if (nfds < 1)
        die("kevent");
    if (kev.ident != vnode_fd ||
            kev.filter != EVFILT_VNODE || 
            kev.fflags != NOTE_RENAME)
        err(1, "%s - incorrect event (sig=%u; filt=%d; flags=%d)", 
                test_id, (unsigned int)kev.ident, kev.filter, kev.flags);

    testfile_rename(1);

    test_no_kevents(kqfd);
}

void
test_kevent_vnode_del(void)
{
    struct kevent kev;

    kevent_add(kqfd, &kev, vnode_fd, EVFILT_VNODE, EV_DELETE, 0, 0, NULL);
}

void
test_kevent_vnode_disable_and_enable(void)
{
    struct kevent kev;
    int nfds;

    test_no_kevents(kqfd);

    /* Add the watch and immediately disable it */
    kevent_add(kqfd, &kev, vnode_fd, EVFILT_VNODE, EV_ADD | EV_ONESHOT, NOTE_ATTRIB, 0, NULL);
    kev.flags = EV_DISABLE;
    kevent_update(kqfd, &kev);

    /* Confirm that the watch is disabled */
    testfile_touch();
    test_no_kevents(kqfd);

    /* Re-enable and check again */
    kev.flags = EV_ENABLE;
    kevent_update(kqfd, &kev);
    testfile_touch();
    nfds = kevent(kqfd, NULL, 0, &kev, 1, NULL);
    if (nfds < 1)
        die("kevent");
    if (kev.ident != vnode_fd ||
            kev.filter != EVFILT_VNODE || 
            kev.fflags != NOTE_ATTRIB)
        err(1, "%s - incorrect event (sig=%u; filt=%d; flags=%d)", 
                test_id, (unsigned int)kev.ident, kev.filter, kev.flags);
}

#if HAVE_EV_DISPATCH
void
test_kevent_vnode_dispatch(void)
{
    struct kevent kev;
    int nfds;

    test_no_kevents(kqfd);

    kevent_add(kqfd, &kev, vnode_fd, EVFILT_VNODE, EV_ADD | EV_DISPATCH, NOTE_ATTRIB, 0, NULL);

    testfile_touch();

    nfds = kevent(kqfd, NULL, 0, &kev, 1, NULL);
    if (nfds < 1)
        die("kevent");
    if (kev.ident != vnode_fd ||
            kev.filter != EVFILT_VNODE || 
            kev.fflags != NOTE_ATTRIB)
        err(1, "%s - incorrect event (sig=%u; filt=%d; flags=%d)", 
                test_id, (unsigned int)kev.ident, kev.filter, kev.flags);

    /* Confirm that the watch is disabled automatically */
    testfile_touch();
    test_no_kevents(kqfd);

    /* Re-enable the kevent */
    /* FIXME- is EV_DISPATCH needed when rearming ? */
    kevent_add(kqfd, &kev, vnode_fd, EVFILT_VNODE, EV_ENABLE | EV_DISPATCH, 0, 0, NULL);
    kev.flags = EV_ADD | EV_DISPATCH;   /* FIXME: may not be portable */
    kev.fflags = NOTE_ATTRIB;
    testfile_touch();
    kevent_cmp(&kev, kevent_get(kqfd));
    test_no_kevents(kqfd);

    /* Delete the watch */
    kevent_add(kqfd, &kev, vnode_fd, EVFILT_VNODE, EV_DELETE, NOTE_ATTRIB, 0, NULL);
}
#endif 	/* HAVE_EV_DISPATCH */

void
test_evfilt_vnode(int _kqfd)
{

#if (defined(__sun) && !defined(HAVE_PORT_SOURCE_FILE))
    puts("**NOTE** EVFILT_VNODE is not supported on this version of Solaris");
    return;
#endif

    snprintf(testfile, sizeof(testfile), "/tmp/kqueue-test%d.tmp",
            testing_make_uid());

	kqfd = _kqfd;
    test(kevent_vnode_add);
    test(kevent_vnode_del);
    test(kevent_vnode_disable_and_enable);
#if HAVE_EV_DISPATCH
    test(kevent_vnode_dispatch);
#endif
    test(kevent_vnode_note_write);
    test(kevent_vnode_note_attrib);
    test(kevent_vnode_note_rename);
    test(kevent_vnode_note_delete);
    unlink(testfile);
}
