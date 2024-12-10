static int io_uintr_register(struct io_ring_ctx *ctx, void __user *arg)
{
	__s32 __user *fds = arg;
	int fd;

	if (ctx->cq_uintr_f)
		return -EBUSY;

	if (copy_from_user(&fd, fds, sizeof(*fds)))
		return -EFAULT;

	ctx->cq_uintr_f = uvecfd_fget(fd);
	if (IS_ERR(ctx->cq_uintr_f)) {
		int ret = PTR_ERR(ctx->cq_uintr_f);

		ctx->cq_uintr_f = NULL;
		return ret;
	}

	return 0;
}

static int io_uintr_unregister(struct io_ring_ctx *ctx)
{
	if (ctx->cq_uintr_f) {
		fput(ctx->cq_uintr_f);
		ctx->cq_uintr_f = NULL;
		return 0;
	}

	return -ENXIO;
}


