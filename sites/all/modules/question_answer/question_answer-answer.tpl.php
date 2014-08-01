<div class="question_answer-answer">
  <div class="question_answer-answer-content"><?php print $comment->comment_body[$node->language][0]['value'];?></div>
  <div class="question_answer-answer-author"><?php echo l(t('@date by @user', array('@date' => format_date($comment->changed), '@user' => $comment->name)), 'node/'. $node->nid . (module_exists('talk') && talk_activated($node->type) ? '/talk' : ''), array('fragment' => 'comment-'.$comment->cid)); ?></div>
</div>
