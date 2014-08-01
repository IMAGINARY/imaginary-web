(function ($, Drupal) {

    Drupal.behaviors.relation_wizard_ui = {};
    Drupal.behaviors.relation_wizard_ui.attach = function (context, settings) {
        $('#relation-wizard-ui-add-form-step2 table.relation-wizard-ui-endpoints').once('relation-ui-dt', function () {
            var rel = $(this).attr('rel');
            var relation = rel.split("|");
            var oTable = $(this).dataTable({
                "bProcessing":true,
                "bServerSide":true,
                "sAjaxSource":Drupal.settings.basePath + "relation_wizard_ui/datatable_ajax?rel_type=" + relation[0] + "&rel_direction=" + relation[1],
                "iDisplayLength":5,
                "bLengthChange":false,
                "aoColumns":[
                    {
                        "bSortable":false
                    },
                    {
                        "bSortable":false
                    },
                    {
                        "bSearchable":false,
                        "bVisible":false,
                        "bSortable":false
                    }
                ],
                "fnDrawCallback": function( oSettings ) {
                    if (oSettings) {
                        oTable.$('tr').click(function () {
                            var data = oTable.fnGetData(this);
                            var value = data[0] + ' [' + data[1] + ':' + data[2] + ']';
                            $('input[id=edit-relation-endpoint]').val(value);

                            if ($(this).hasClass('row_selected')) {
                                $(this).removeClass('row_selected');
                            }
                            else {
                                oTable.$('tr.row_selected').removeClass('row_selected');
                                $(this).addClass('row_selected');
                            }
                        });
                    }
                }
            });

        });
    };

})(jQuery, Drupal);