#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>	/* printk() */
#include <linux/slab.h>   	/* kmalloc() */
#include <linux/fs.h>	  	/* register_chrdev() */
#include <linux/errno.h>	/* error codes */
#include <linux/types.h>	/* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>	/* O_ACCMODE */
#include <asm/uaccess.h>	/* copy_from/to_user */
#include <linux/cdev.h>

MODULE_LICENSE("Dual BSD/GPL");

/* Declaration of memory.c functions */
int msgbox_open(struct inode *inode, struct file *filp);
int msgbox_release(struct inode *inode, struct file *filp);
ssize_t msgbox_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
ssize_t msgbox_write(struct file *filp, char *buf, size_t count, loff_t *f_pos);
void msgbox_exit(void);
int msgbox_init(void);

/* Structure that declares the usual file */
/* access functions */
struct file_operations msgbox_fops = {
	read: msgbox_read,
	write: msgbox_write,
	open: msgbox_open,
	release: msgbox_release
};

struct message {
	char *to, *data;
};

struct msgbox_dev {
	struct message* messages;
	unsigned msg_count;
	struct cdev cdev;
};

/* Global variables of the driver */
int msgbox_major = 61; 		/* major number */
struct message *msg_buffer;	/* buffer to store message data */
struct msgbox_dev *messagebox;	/* messagebox device */

int msgbox_init(void){
		int result;
		dev_t devno;
		int err;
		
		/* Register device */
		result = register_chrdev(msgbox_major, "msgbox", &msgbox_fops);
		if(result < 0){
			printk("msgbox: cannot obtain major number %d\n",msgbox_major);
			return result;
		}
		/* MESSAGE BUFFER */
		/* Allocating memory for the buffer */
		msg_buffer = kmalloc(sizeof(struct message), GFP_KERNEL);
		if(!msg_buffer){
			result = -ENOMEM;
			goto fail;
		}
		memset(msg_buffer, 0, sizeof(struct message));
		
		/* initialize buffer */
		msg_buffer->data = kmalloc(100*sizeof(char), GFP_KERNEL);
		if(!msg_buffer->data){
			result = -ENOMEM;
			goto fail;
		}
		memset(msg_buffer->data, 0, 100*sizeof(char));
		
		msg_buffer->to = kmalloc(20*sizeof(char), GFP_KERNEL);
		if(!msg_buffer->to){
			result = -ENOMEM;
			goto fail;
		}
		memset(msg_buffer->to, 0, 20*sizeof(char));
		
		
		/* MESSAGEBOX */
		/* allocate memory for message box */
		messagebox = kmalloc(sizeof(struct msgbox_dev), GFP_KERNEL);
		if(!messagebox){
			result = -ENOMEM;
			goto fail;
		}
		memset(messagebox, 0, sizeof(struct msgbox_dev));
		
		/* initialize messagebox data */
		messagebox->msg_count = 0;
		messagebox->messages = kmalloc(3*sizeof(struct message), GFP_KERNEL);
		if(!messagebox->messages){
			result = -ENOMEM;
			goto fail;
		}
		memset(messagebox->messages, 0, 3*sizeof(struct message));
		
		messagebox->msg_count = 0;
		devno = MKDEV(61, 0);
		cdev_init(&messagebox->cdev, &msgbox_fops);
		messagebox->cdev.ops = &msgbox_fops;
		err = cdev_add(&messagebox->cdev, devno, 1);
        if (err)
            printk(KERN_NOTICE "Error %d adding msgbox", err);
		
		printk("Inserting msgbox module\n");
		return 0;
		

		fail: 
			msgbox_exit();
			return result;
}

void msgbox_exit(void){
	/* Freeing the major number */
	unregister_chrdev(msgbox_major, "memory");
	
	/* Freeing buffer memory */
	if(msg_buffer->data)
		kfree(msg_buffer->data);
		
	if(msg_buffer->to)
		kfree(msg_buffer->to);
	
	if(msg_buffer){
		kfree(msg_buffer);
	}
	
	if(messagebox->messages){
		kfree(messagebox->messages);
	}
	
	if(messagebox)
		kfree(messagebox);
	
	printk("Removing msgbox module\n");
}

int msgbox_open(struct inode *inode, struct file *filp){
	/*
	 * When a file is opened, it is normally necessary to initialize
	 * driver variables or reset the device. In this simple example
	 * though, these operations are not performed.
	 * 	
	 */ 
	  
	/* success */
	return 0;
}

int msgbox_release(struct inode *inode, struct file *filp){
	/* success */
	return 0;
}

ssize_t msgbox_read(struct file *filp, char *buf, size_t count, loff_t *f_pos){
	ssize_t retval = 0;
	int i;
	
	if (*f_pos + count > 100)	// 100 -> max size of a msg
        count = 100 - *f_pos;
	if(msg_buffer->data == NULL){
		printk("buffer null\n");
		goto out;
	}
	
	printk("in read, count = %d\n", count);
	/* Transfering data to user space */
	//copy_to_user(buf, msg_buffer->data, count);  /* char count */
	/*for(i=0 ; i<messagebox->msg_count ; i++){
		if (copy_to_user(buf, messagebox->messages[i].data, count)) {
			retval = -EFAULT;
			goto out;
		}
	}*/
	
	if (copy_to_user(buf, msg_buffer->data, count)) {
		retval = -EFAULT;
		goto out;
	}
	
	*f_pos += count;
	retval = count;
	
	out:
	return retval;
}

ssize_t msgbox_write(struct file *filp, char *buf, size_t count, loff_t *f_pos){
	int i = 0, begin, size = 0;
	//struct msgbox_dev *dev = filp->private_data;
	//printk("dev assigned \n");
	if(*f_pos >= count)
		goto out;

	//printk("count=%d\n", messagebox->msg_count); //segmentation
	/* extract the name from buf */
	while(i<count){
		if(buf[i] == ' ')
			break;
		i++;
	}
	begin = ++i;	/* begining of the name, after ' ' */
	
	while(buf[i++] != ':') size++;	/* end of the name: begin + size */
	size++;
	if(size < 20){
		for(i=0 ; i < size ; ++i){
			msg_buffer->to[i] = buf[begin+i];
			//dev->messages[dev->msg_count].to[i] = buf[begin+i];
		}
		//dev->messages[dev->msg_count].to[i] = '\0'; /* just in case */
		msg_buffer->to[i] = '\0'; /* just in case */
	}
	printk("name extracted\n");
	
	/* buf[begin+size+1] --> skips "to: name " part of the string
	 * count - (begin+size+1) --> keeps the boundary
	 * thus msg_buffer->data gets only the text part of the echo
	 */
	
	copy_from_user(msg_buffer->data, &buf[begin+size+1], count-(begin+size+1));
	
	/*copy_from_user(dev->messages[dev->msg_count].data, 
		&buf[begin+size+1], count-(begin+size+1));
	*/
	
	printk("copied from user\n");
		
	//messagebox->msg_count++;
	//printk("msg_count = %d\n", dev->msg_count);
	
	
	*f_pos += count;
	out:
	return 1;
}

/* Declaration of the init and exit functions */
module_init(msgbox_init);
module_exit(msgbox_exit);
