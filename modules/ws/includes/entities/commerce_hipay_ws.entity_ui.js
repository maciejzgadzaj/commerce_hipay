(function($) {

Drupal.behaviors.accountFieldsetSummaries = {
  attach: function (context, settings) {
    $('fieldset#edit-user', context).drupalSetSummary(function (context) {
      var name = $('#edit-name').val() || Drupal.settings.anonymous;

      return Drupal.t('Owned by @name', { '@name': name });
    });
  }
};

})(jQuery);
